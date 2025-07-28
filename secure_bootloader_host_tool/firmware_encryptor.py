import os
import hashlib
import secrets
from datetime import datetime
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives.asymmetric.utils import Prehashed, decode_dss_signature
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import struct

METADATA_TOTAL_SIZE = 256  # total fixed metadata size
VALID_METADATA_SIZE = 4 + 4 + 4 + 16 + 32 + 64 + 16  # 140 bytes

# Paths
ROOT_DIR = os.path.dirname(__file__)
KEY_DIR = os.path.join(ROOT_DIR, "Keys")
INPUT_BIN = os.path.join(ROOT_DIR, "../Stm32F446reFreeRtos_Application/Debug/Stm32F446reFreeRtos_Application.bin")
OUTPUT_BIN = os.path.join(ROOT_DIR, "../Stm32F446reFreeRtos_Application/Debug/Stm32F446reFreeRtos_Application_withMetadata.bin")
PRIVATE_KEY_PATH = os.path.join(KEY_DIR, "private_key.pem")
AES_KEY_PATH = os.path.join(KEY_DIR, "aes_key.bin")   # 16 bytes

def load_private_key(path):
    with open(path, "rb") as f:
        return serialization.load_pem_private_key(f.read(), password=None, backend=default_backend())

def load_aes_key(path):
    with open(path, "rb") as f:
        return f.read()

def sign_hash(private_key, hash_bytes):
    signature_der = private_key.sign(hash_bytes, ec.ECDSA(Prehashed(hashes.SHA256())))
    r, s = decode_dss_signature(signature_der)
    return r.to_bytes(32, 'big') + s.to_bytes(32, 'big')  # 64 bytes

def encrypt_firmware(firmware_bytes, aes_key):
    iv = secrets.token_bytes(16)
    cipher = Cipher(algorithms.AES(aes_key), modes.CBC(iv), backend=default_backend())
    encryptor = cipher.encryptor()

    pad_len = 16 - (len(firmware_bytes) % 16)
    firmware_padded = firmware_bytes + bytes([pad_len] * pad_len)

    encrypted = encryptor.update(firmware_padded) + encryptor.finalize()
    return iv, encrypted, len(firmware_bytes), len(encrypted)


def write_output(enc_fw, fw_size_raw, fw_size_enc, iv, sha256_hash, signature, output_path):
    with open(output_path, "wb") as f:
        f.write(enc_fw)                              # Encrypted firmware first
        f.write(struct.pack("<I", fw_size_enc))      # 4 bytes Encrypted FW size
        f.write(struct.pack("<I", fw_size_raw))      # 4 bytes Raw FW size
        padding_len = fw_size_enc - fw_size_raw
        f.write(struct.pack("<I", padding_len))      # 4 bytes Padding size
        f.write(iv)                                  # 16 bytes AES IV
        f.write(sha256_hash)                         # 32 bytes SHA256
        f.write(signature)                           # 64 bytes ECC Signature
        f.write(b'\x00' * 16)                        # 16 bytes Reserved
        f.write(b'\xFF' * (METADATA_TOTAL_SIZE - VALID_METADATA_SIZE))  # Padding



def print_debug_info(fw_size_orig, fw_size_enc, iv, sha256_hash, signature):
    print("\n Firmware Encryption Summary:")
    print(f"   • Raw firmware size       : {fw_size_orig} bytes")
    print(f"   • Encrypted firmware size : {fw_size_enc} bytes")
    print(f"   • Padded (ency-org) byets : {fw_size_enc - fw_size_orig} bytes")
    print(f"   • AES IV                  : {iv.hex()}")
    print(f"   • SHA256                  : {sha256_hash.hex()} bytes")
    print(f"   • ECC signature           : {signature.hex()}")
    print(f"   • Reserved                : {16} bytes")
    print(f"   • Padded bytes (Meta)     : {METADATA_TOTAL_SIZE - VALID_METADATA_SIZE} bytes")
    print(f"\n Output File: {os.path.relpath(OUTPUT_BIN)}")

def main():
    import time
    start_time = time.time()

    with open(INPUT_BIN, "rb") as f:
        firmware = f.read()

    # Load Keys
    private_key = load_private_key(PRIVATE_KEY_PATH)
    aes_key = load_aes_key(AES_KEY_PATH)

    # Calculate tthe SHA 256
    sha256_hash = hashlib.sha256(firmware).digest()

    #ECC signature against the SHA hash
    signature = sign_hash(private_key, sha256_hash)
    
    # AES 128 CBC Encryption the Loaded Key
    iv, encrypted_fw, fw_size_orig, fw_size_enc = encrypt_firmware(firmware, aes_key)

    # rest reserved metadata  

    # Write into xx_withMetadata.bin
    write_output(encrypted_fw, fw_size_orig, fw_size_enc,iv, sha256_hash, signature, OUTPUT_BIN)

    #debug statements
    print_debug_info(fw_size_orig, fw_size_enc, iv, sha256_hash, signature)

    elapsed_time = time.time() - start_time
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f"\n Firmware Encryption completed at {timestamp} in {elapsed_time:.2f} seconds\n")

if __name__ == "__main__":
    main()

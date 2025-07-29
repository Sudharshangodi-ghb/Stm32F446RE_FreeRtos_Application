import os
import struct
import hashlib
import base64
import os
import base64
from datetime import datetime
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.exceptions import InvalidSignature
from datetime import datetime
from cryptography.hazmat.primitives.asymmetric.utils import encode_dss_signature


# Constants
METADATA_TOTAL_SIZE = 256   # bytes
SIGNATURE_SIZE = 64         # ECDSA (P-256)
AES_IV_SIZE = 16            # AES-CBC IV
SHA256_SIZE = 32            # bytes

# AES block size
AES_BLOCK_SIZE = 16


# Paths
KEYS_DIR = os.path.join(os.path.dirname(__file__), "Keys")
PUBLIC_KEY_PATH = os.path.join(KEYS_DIR, "public_key.pem")
AES_KEY_PATH = os.path.join(KEYS_DIR, "aes_key.bin")

def load_public_key(path):
    with open(path, 'rb') as f:
        key_data = f.read()
        return serialization.load_pem_public_key(key_data)

def load_aes_key(key_path):
    with open(key_path, "rb") as f:
        key = f.read()
    assert len(key) == 16, "AES key must be 16 bytes for AES-128"
    return key


def extract_metadata(full_path):
    with open(full_path, "rb") as f:
        data = f.read()
        metadata = data[-METADATA_TOTAL_SIZE:]  # Last 256 bytes
        enc_fw_data = data[:-METADATA_TOTAL_SIZE]

        offset = 0
        enc_fw_size = struct.unpack("<I", metadata[offset:offset+4])[0]
        offset += 4
        firmware_size = struct.unpack("<I", metadata[offset:offset+4])[0]
        offset += 4
        padding_size = struct.unpack("<I", metadata[offset:offset+4])[0]
        offset += 4
        aes_iv = metadata[offset:offset+16]
        offset += 16
        sha256_hash = metadata[offset:offset+32]
        offset += 32
        signature = metadata[offset:offset+64]
        offset += 64
        reserved = metadata[offset:offset+16]
        offset += 16
        # Remaining 116 bytes can be read if needed


    return enc_fw_size, sha256_hash, signature, aes_iv

def verify_signature(public_key, computed_hash, signature):
    try:
        public_key.verify(signature, computed_hash, ec.ECDSA(hashes.SHA256()))
        return True
    except InvalidSignature:
        return False

def verify_sha256(firmware_data, extracted_hash):
    computed_hash = hashlib.sha256(firmware_data).digest()
    return computed_hash == extracted_hash, computed_hash

def convert_raw_signature_to_der(raw_signature):
    r = int.from_bytes(raw_signature[:32], 'big')
    s = int.from_bytes(raw_signature[32:], 'big')
    return encode_dss_signature(r, s)

def main():
    bin_dir = os.path.join(os.path.dirname(__file__), "../Stm32F446reFreeRtos_Application/Debug/")
    bin_file = None

    # Auto-detect .bin file ending with _withMetadata.bin
    for file in os.listdir(bin_dir):
        if file.endswith("_withMetadata.bin"):
            bin_file = os.path.join(bin_dir, file)
            break

    if not bin_file:
        print(" No _withMetadata.bin file found.")
        return

    print(f"\n Reading: {os.path.basename(bin_file)}")

    enc_fw_size, sha256_hash, signature, aes_iv = extract_metadata(bin_file)

    # Load keys
    public_key = load_public_key(PUBLIC_KEY_PATH)
    # Load AES Key
    aes_key = load_aes_key(AES_KEY_PATH)

    # Extract firmware content (without metadata)
    with open(bin_file, 'rb') as f:
        firmware_data_enc = f.read(enc_fw_size)

    print("\n Extracted Metadata:")
    print(f"  → Enc Firmware Size   : {enc_fw_size} bytes")
    print(f"  → SHA256 Hash         : {sha256_hash.hex()}")
    print(f"  → ECC Signature       : {signature.hex()}")
    print(f"  → AES IV              : {aes_iv.hex()}")

    signature_der = convert_raw_signature_to_der(signature)




    # Decrypt AES-CBC
    cipher = AES.new(aes_key, AES.MODE_CBC, aes_iv)
    try:
        firmware_data = unpad(cipher.decrypt(firmware_data_enc), AES_BLOCK_SIZE)
        print("\n Firmware Decryption: SUCCESS")
    except ValueError:
        print("\n Firmware Decryption: FAILED (Invalid padding or key?)")
        return
    

    # Verify SHA256
    sha_match, computed_hash = verify_sha256(firmware_data, sha256_hash)
    print("\n SHA256 Check          :", "MATCHED" if sha_match else "MISMATCH")
    print(f"  → Computed Hash       : {computed_hash.hex()}")

    # Verify ECC Signature
    is_signature_valid = verify_signature(public_key, computed_hash, signature_der)
    print(" ECC Signature Check:", "VALID" if is_signature_valid else "INVALID")

    # Print Key Info Summary
    print("\n Keys Used:")
    print(f"  → Public Key          : ../Keys/public_key.pem")
    print(f"  → AES Key             : ../Keys/aes_key.pem")

    # Timestamp
    print(f"\n Timestamp            : {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")

if __name__ == "__main__":
    main()

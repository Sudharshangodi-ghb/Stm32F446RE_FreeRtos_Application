import os
import time
from datetime import datetime
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
import secrets

# Directory to save keys
KEY_DIR = os.path.join(os.path.dirname(__file__), "Keys")
os.makedirs(KEY_DIR, exist_ok=True)

# Output file paths
PRIVATE_KEY_PATH = os.path.join(KEY_DIR, "private_key.pem")
PUBLIC_KEY_PATH = os.path.join(KEY_DIR, "public_key.pem")
AES_KEY_PATH = os.path.join(KEY_DIR, "aes_key.bin")  # <-- fixed to .bin

def save_pem_file(file_path, data, label):
    with open(file_path, "wb") as f:
        f.write(data)
    print(f"  {label} saved to: {os.path.relpath(file_path)}")

def generate_ecc_keys():
    print(" Generating ECC key pair (secp256r1)...")
    private_key = ec.generate_private_key(ec.SECP256R1(), default_backend())
    public_key = private_key.public_key()

    private_bytes = private_key.private_bytes(
        serialization.Encoding.PEM,
        serialization.PrivateFormat.TraditionalOpenSSL,
        serialization.NoEncryption()
    )
    save_pem_file(PRIVATE_KEY_PATH, private_bytes, "Private Key")

    public_bytes = public_key.public_bytes(
        serialization.Encoding.PEM,
        serialization.PublicFormat.SubjectPublicKeyInfo
    )
    save_pem_file(PUBLIC_KEY_PATH, public_bytes, "Public Key")

def generate_aes_key(key_size=16):
    print(f" Generating AES-{key_size * 8} key...")
    aes_key = secrets.token_bytes(key_size)
    with open(AES_KEY_PATH, "wb") as f:
        f.write(aes_key)
    print(f"  AES Key saved to: {os.path.relpath(AES_KEY_PATH)}")

def main():
    start_time = time.time()
    print("\n Starting Secure Key Generation...\n")

    generate_ecc_keys()
    generate_aes_key(16)  # AES-128 (16 bytes)

    elapsed_time = time.time() - start_time
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f"\n Key generation completed at {timestamp} in {elapsed_time:.2f} seconds\n")

if __name__ == "__main__":
    main()

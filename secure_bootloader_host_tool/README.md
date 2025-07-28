
# File  management
📁 root_directory/
├── 📁 secure_bootloader_host_tool/
│   ├── 📁 Keys/
│   │   ├── private_key.pem
│   │   ├── public_key.pem
│   │   └── aes_key.pem
│   ├── firmware_encryptor.py
│   ├── view_metadata.py
│   └── generate_keys.py
├── 📁 Stm32F446reFreeRtos_Application/
│   └── 📁 Debug/
│       ├── Stm32F446reFreeRtos_Application.bin
│       └── Stm32F446reFreeRtos_Application_withMetadata.bin


# Metadata Layout – 256 bytes

OFFSET | SIZE    | FIELD
-------|---------|-----------------------------
0x00   | 4       | firmware_size (uint32)
0x04   | 32      | sha256_hash
0x24   | 64/72   | ecc_signature (depends on encoding)
0x68   | 16      | aes_iv
0x78   | 4       | metadata_marker (e.g., b'META')
0x7C   | 4       | metadata_version (e.g., 0x00000001)
0x80   | (fill)  | Reserved (zero-padding)


# 1. generate_keys.py
This script will:
    ECDSA key pair (private and public) using the SECP256R1 curve (NIST P-256).
    AES key (128-bit) stored as a .pem file.
    Output files in secure_bootloader_host_tool/Keys/:
        private_key.pem
        public_key.pem
        aes_key.bin

The ECC private key is correct and follows secp256r1 standards.
The public key is derived from it and correctly encoded.
Using AES-128 (16 bytes) written in aes_key.bin — a raw key usable for firmware encryption.
Everything generated from "generate_keys.py"



# 2. firmware_encryptor.py
This script will:
    Read Stm32F446reFreeRtos_Application.bin
    Generate:
        Encrypt the raw firmware using AES-CBC.
        Compute SHA256 over encrypted data.
        Sign that hash using ECC private key.
        Append metadata to create _withMetadata.bin.
    Append metadata to firmware
    Write Stm32F446reFreeRtos_Application_withMetadata.bin

# Stm32F446reFreeRtos_Application_withMetadata.bin
| Field                  | Size (Bytes)     | Description                                    |
| ---------------------- | -----------------| ---------------------------------------------- |
|   Encrypted Firmware   | M                | AES-128-CBC encrypted firmware (with padding). |
|   AES IV               | 16               | Random IV used during AES encryption.          |
|   Encrypted FW Size    | 4                | Size of encrypted firmware (`M`).              |
|   Original FW Size     | 4                | Raw firmware size (`N`).                       |
|   Padding Size         | 4                | Extra bytes added for PKCS7 padding.           |
|   SHA256               | 32               | Hash of the original raw firmware.             |
|   ECC Signature        | 64               | ECDSA signature over the SHA256 hash.          |




# 3. view_metadata.py
This script will:
    Read _withMetadata.bin
    Extract:
        Firmware size
        SHA256 hash
        ECC signature
        AES IV (randomly generated during encryption)
    Show info in human-readable format using public_key.pem and aes_key.pem.


1.Review the metadata parser logic:
    SHA256 hash (32 bytes)
    ECC signature (64 bytes, if secp256r1)
    Firmware size
    AES IV (16 bytes, if used)
2.Check correctness of:
    Byte offsets
    Signature length and ECC format assumptions
    SHA256 hash display
    Output clarity
3.Ensure compatibility with your firmware_encryptor.py output.

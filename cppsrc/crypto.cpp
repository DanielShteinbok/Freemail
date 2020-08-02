#include <napi.h>
#include <sodium.h>
#include "crypto.h"

using namespace crypto;

Napi::Object CryptoBoxKeypairWrapped(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	unsigned char *recipient_pk = new unsigned char [crypto_box_PUBLICKEYBYTES];
	unsigned char *recipient_sk = new unsigned char [crypto_box_SECRETKEYBYTES];
	crypto_box_keypair(recipient_pk, recipient_sk);
	Napi::ArrayBuffer public_key = Napi::ArrayBuffer::New(env, recipient_pk, crypto_box_PUBLICKEYBYTES,
		[](void *externalData) -> void 
		{
			delete [] externalData;
		}
	);
	Napi::ArrayBuffer secret_key = Napi::ArrayBuffer::New(env, recipient_sk, crypto_box_SECRETKEYBYTES,
		[](void *externalData) -> void 
		{
			delete [] externalData;
		}
	);
	Napi::Object toReturn = Napi::Object::New(env);
	toReturn.Set("public_key", public_key);
	toReturn.Set("secret_key", secret_key);
	return toReturn;
}

Napi::ArrayBuffer CryptoBoxSealWrapped(const Napi::CallbackInfo &info)
{
	Napi::Env env = info.Env();
	// input taken as:
	// info[0] = plaintext message as an ArrayBuffer
	// info[1] = encryption key / recipient public key also as an ArrayBuffer
	if (info.Length() != 2 || !info[0].IsArrayBuffer() || !info[1].IsArrayBuffer())
		Napi::TypeError::New(env, "Incorrect input").ThrowAsJavaScriptException();
	size_t encryptedLength = info[0].As<Napi::ArrayBuffer>().ByteLength() + crypto_box_SEALBYTES;
	unsigned char *encrypted_message = new unsigned char [encryptedLength];
	unsigned char *plaintextMessage = static_cast<unsigned char*>(info[0].As<Napi::ArrayBuffer>().Data());
	unsigned char *encryptionKey = static_cast<unsigned char*>(info[1].As<Napi::ArrayBuffer>().Data());
	crypto_box_seal(encrypted_message, plaintextMessage, info[0].As<Napi::ArrayBuffer>().ByteLength(), encryptionKey);
	return Napi::ArrayBuffer::New(env, encrypted_message, encryptedLength, 
		[](void *externalData) -> void 
		{
			delete [] externalData;
		}	
	);
}
		
Napi::ArrayBuffer CryptoBoxSealOpenWrapped(const Napi::CallbackInfo &info)
{
	
	Napi::Env env = info.Env();
	// input taken as:
	// info[0] = ciphertext message as an ArrayBuffer
	// info[1] = decryption key / recipient private key also as an ArrayBuffer
	if (info.Length() != 3 || !info[0].IsArrayBuffer() || !info[1].IsArrayBuffer() || !info[2].IsArrayBuffer())
		Napi::TypeError::New(env, "Incorrect input").ThrowAsJavaScriptException();
	size_t decryptedLength = info[0].As<Napi::ArrayBuffer>().ByteLength() - crypto_box_SEALBYTES;
	unsigned char *decrypted_message = new unsigned char [decryptedLength];
	unsigned char *ciphertextMessage = static_cast<unsigned char*>(info[0].As<Napi::ArrayBuffer>().Data());
	unsigned char *decryptionKey = static_cast<unsigned char*>(info[1].As<Napi::ArrayBuffer>().Data());
	unsigned char *publicKey = static_cast<unsigned char*>(info[2].As<Napi::ArrayBuffer>().Data());
	crypto_box_seal_open(decrypted_message, ciphertextMessage, info[0].As<Napi::ArrayBuffer>().ByteLength(), decryptionKey, publicKey);
	return Napi::ArrayBuffer::New(env, decrypted_message, decryptedLength,
		[](void *externalData) -> void 
		{
			delete [] externalData;
		}
	);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
	exports.Set("crypto_box_keypair", Napi::Function::New(env, CryptoBoxKeypairWrapped));
	exports.Set("crypto_box_seal", Napi::Function::New(env, CryptoBoxSealWrapped));
	exports.Set("crypto_box_seal_open", Napi::Function::New(env, CryptoBoxSealOpen));
	return exports;
}

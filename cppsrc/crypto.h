#ifndef CRYPTO_H
#define CRYPTO_H
#include <napi.h>

namespace crypto
{

	Napi::Object CryptoBoxKeypairWrapped(const Napi::CallbackInfo &info);
	Napi::ArrayBuffer CryptoBoxSealWrapped(const Napi::CallbackInfo &info);
	Napi::ArrayBuffer CryptoBoxSealOpenWrapped(const Napi::CallbackInfo &info);

	Napi::Object Init(Napi::Env env, Napi::Object exports);

}
#endif

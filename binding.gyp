{
    "targets": [{
        "target_name": "crypto_addon",
	#"conditions": [
	#	['OS=="linux"', {
	#		'ldflags': [
	#			'-lsodium',
	#		],
	#	}],
	#],
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
		"cppsrc/main.cpp",
		"cppsrc/crypto.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': ['-lsodium'],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}

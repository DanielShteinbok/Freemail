cmd_Release/obj.target/daft_protocol.node := g++ -shared -pthread -rdynamic -m64  -Wl,-soname=daft_protocol.node -o Release/obj.target/daft_protocol.node -Wl,--start-group Release/obj.target/daft_protocol/cppsrc/main.o Release/obj.target/node_modules/node-addon-api/nothing.a -Wl,--end-group -lsodium
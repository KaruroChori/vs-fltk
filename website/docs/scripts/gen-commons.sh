rm -rf ./commons
cp -r ../commons ./

mkdir -p ./commons/bindings/native/tcc
cp -r ../subprojects/libtcc/include/ ./commons/bindings/native/tcc

mkdir -p ./commons/bindings/
cp -r ../bindings/ ./commons/

mkdir -p ./commons/docs/
cp -r ../docs/ ./commons/

mkdir -p ./commons/examples/
cp -r ../examples/ ./commons/
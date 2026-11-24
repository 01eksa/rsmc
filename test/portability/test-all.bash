SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)

if [ "$(readelf -d build/release/librsmc.so | grep NEEDED)" ]; then
    echo 'Found dependency!'
    echo "$(readelf -d build/release/librsmc.so)"
    exit 1
fi


for df in "$SCRIPT_DIR"/*.Dockerfile; do
    docker build -t rsmc-test -f "$df" "$(dirname -- "$df")"
    docker run --rm -v "$PROJECT_ROOT:/workdir" rsmc-test sh "test/portability/test-precompiled.sh"
    if [ $? -ne 0 ]; then
        echo "FAIL on $df"
        exit 1
    fi
done
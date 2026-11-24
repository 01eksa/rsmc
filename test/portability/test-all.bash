SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/../.." && pwd)

for df in "$SCRIPT_DIR"/*.Dockerfile; do
    docker build -t rsmc-test -f "$df" "$(dirname -- "$df")"
    docker run --rm -v "$PROJECT_ROOT:/workdir" rsmc-test sh "test/portability/test-precompiled.sh"
done
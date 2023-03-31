#!/bin/bash

ROOT_DIR="$(readlink -f $(dirname $BASH_SOURCE)/..)"

source ${ROOT_DIR}/build.properties

PACKAGE_ROOT=${ROOT_DIR}/package
PACKAGE_VERSION=${version_major}.${version_minor}.${version_patch}
PACKAGE_NAME=${package_name}
PACKAGE_FULLNAME=${PACKAGE_NAME}-${PACKAGE_VERSION}
PACKAGE_PATH=${PACKAGE_ROOT}/${PACKAGE_FULLNAME}
PACKAGE_TAR=${PACKAGE_PATH}.tgz

echo "PACKAGE_TAR: $PACKAGE_TAR"

function copy() {
    local path="$1"
    local dest=$PACKAGE_PATH/$path

    mkdir -p $dest

    find $path -maxdepth 1 -type f -regex ".*\.\(hpp\|cpp\|md\|csv\|npy\|sh\|txt\)$" -exec cp {} $dest \;
}

function create_package() {
    rm -rf ${PACKAGE_PATH}
    rm -f ${PACKAGE_TAR}
    mkdir -p ${PACKAGE_PATH}
    cd ${PACKAGE_ROOT} || return 1
    rm -f ${PACKAGE_NAME}
    ln -s ${PACKAGE_FULLNAME} ${PACKAGE_NAME}
    cd ${ROOT_DIR} || return 1

    FOLDERS=(
        .
        include
        include/pd
        include/pd/core
        include/pd/core/frame
        include/pd/core/frame/DataFrame
        include/pd/core/internal
        include/pd/core/series
        include/pd/core/series/Series
        samples
        samples/read_csv
        scripts
        src
        src/pd
        src/pd/core
        src/pd/core/frame
        src/pd/core/frame/DataFrame
        src/pd/core/series
        src/pd/core/series/Series
        unit_tests
        unit_tests/include
        unit_tests/src
    )
    for folder in "${FOLDERS[@]}"; do
        copy $folder
    done

    return 0
}

function zip_package() {
    rm -f ${PACKAGE_TAR} || return 1
    tar zcf ${PACKAGE_TAR} -C "$(dirname ${PACKAGE_PATH})" "$(basename ${PACKAGE_PATH})"

    return 0
}

function main() {
    create_package || return 1
    zip_package
}

main
#!/usr/bin/env zsh

echo "update everything"
brew update

echo "installing dependencies"
brew install python3@3.9
brew install python3-tk@3.9
brew install numpy
brew install glew
brew install gcc binutils bison
brew install cmake
brew install pkg-config
brew install libxi
brew install libxmu
brew install freeglut
brew install wget
brew install glfw

echo "installing c libaries"

export PROJECTDIR=$(pwd)
export ENVDIR=$PROJECTDIR/c_env

mkdir -p $ENVDIR
mkdir -p $ENVDIR/include
mkdir -p $ENVDIR/lib
mkdir -p $ENVDIR/lib/cmake
mkdir -p $ENVDIR/src

install_library() {
    cd $ENVDIR/src
    git clone --depth 1 --branch $3 $2
    echo "==== Installing $1 at $ENVDIR ===="
    mkdir $1/build
    cd $1/build
    if [ -f ../CMakeLists.txt ]; then
        cmake -DCMAKE_BUILD_TYPE=Release \
              -DCMAKE_PREFIX_PATH=$ENVDIR \
              -DCMAKE_INSTALL_PREFIX=$ENVDIR \
              -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE \
              -DCMAKE_INSTALL_RPATH=$ENVDIR \
              $4 \
              ..
    elif [ -f Makefile ]; then
        cd ..
    fi

    make -j8 install
}
install_boost() {
    cd $ENVDIR/src
  wget https://boostorg.jfrog.io/artifactory/main/release/1.71.0/source/boost_1_71_0.tar.gz
  tar -xzf ./boost_1_71_0.tar.gz
  rm ./boost_1_71_0.tar.gz
  cd ./boost_1_71_0
    # git clone --branch boost-1.69.0 https://github.com/boostorg/boost.git
    # cd $ENVDIR/src/boost
 #  git submodule update --init --depth 1 --recursive
    ./bootstrap.sh --with-python=python3 --with-libraries=atomic,chrono,filesystem,python,system,regex,program_options
    ./b2 --with-python --with-filesystem --with-regex --with-system --with-program_options --prefix=$ENVDIR install
}

install_library tinyxml2 https://github.com/leethomason/tinyxml2 8.0.0
install_library eigen https://gitlab.com/libeigen/eigen 3.3.7
#install_library libccd https://github.com/danfis/libccd v2.0
#install_library assimp https://github.com/assimp/assimp v4.0.1
install_boost

brew install pybind11

cd $PROJECTDIR

echo "installing python env"

pip3 install virtualenv virtualenvwrapper
source ./pyenv/bin/activate
pip3 install --upgrade pip
pip3 install -r requirement.txt

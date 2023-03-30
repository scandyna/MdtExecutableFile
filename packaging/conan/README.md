# Create a Conan package

The package version is picked up from git tag.
If working on MdtExecutableFile, go to the root of the source tree:
```bash
git tag x.y.z
conan create packaging/conan/MdtExecutableFileCore scandyna/testing --profile:build $CONAN_PROFILE_BUILD --profile:host $CONAN_PROFILE_HOST --settings:build build_type=Release --settings:host build_type=$BUILD_TYPE
```

To create a package without having a git tag:
```bash
conan create packaging/conan/MdtExecutableFileCore x.y.z@scandyna/testing --profile:build $CONAN_PROFILE_BUILD --profile:host $CONAN_PROFILE_HOST --settings:build build_type=Release --settings:host build_type=$BUILD_TYPE
```

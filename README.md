# 文档

## Eigen

<https://eigen.tuxfamily.org/dox/TopicCMakeGuide.html>

下载源码后，根据 INSTALL 文件执行 cmake 和 make 安装 Eigen 。

> Method 2. Installing using CMake
>
> ---
>
> Let's call this directory 'source_dir' (where this INSTALL file is).
> Before starting, create another directory which we will call 'build_dir'.
>
> Do:
>
> cd build_dir
> cmake source_dir
> make install
>
> The "make install" step may require administrator privileges.
>
> You can adjust the installation destination (the "prefix")
> by passing the -DCMAKE_INSTALL_PREFIX=myprefix option to cmake, as is
> explained in the message that cmake prints at the end.

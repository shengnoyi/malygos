# FAQ

## .obj

```txt
explain following line which is in a .obj file:

f 953/977/953 792/816/792 952/975/952
```

> This line is describing a polygonal face in a 3D object, represented in the Wavefront OBJ file format. The 'f' stands for "face" and is followed by a series of vertex references, which describe the vertices that make up the polygonal face.
>
> Each vertex reference consists of three numbers separated by slashes: the first number is the index of the vertex position in the list of vertices, the second number is the index of the texture coordinate for that vertex (if any), and the third number is the index of the vertex normal for that vertex.
>
> So in this line, the polygonal face is made up of three vertices: the first vertex has a position index of 953, a texture coordinate index of 977, and a normal index of 953; the second vertex has a position index of 792, a texture coordinate index of 816, and a normal index of 792; and the third vertex has a position index of 952, a texture coordinate index of 975, and a normal index of 952.
>
> Overall, this line is specifying a single triangular face in the 3D object, where each vertex is defined by its position, texture coordinate, and normal vector.

重点： position, texture coordinate, and normal vector

## vscode intelligence

<https://stackoverflow.com/questions/57458423/vscode-not-recognizing-includes-from-includepath/75564806#75564806>

> I have had the same issue. In my case, it was an unnecessary line in the c_cpp_properties.json file. That line was: "configurationProvider": "ms-vscode.makefile-tools".
>
> Due to this line, it tried to configure includes from an improperly made Makefile, ignoring "includePath" from the c_cpp_properties.json file.
>
> To solve the issue, comment out the mentioned line or adequately configure your Makefile. After that, a restart of VS Code may be necessary. Try also temporarily hide Makefile (e.g. rename to .Makefile in Linux).

`.vscode/c_cpp_properties.json`:

```json
{
  "configurations": [
    {
      "name": "Mac",
      "includePath": ["${workspaceFolder}/**"],
      "defines": [],
      "macFrameworkPath": [
        "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks"
      ],
      "compilerPath": "/usr/bin/clang",
      "cStandard": "c17",
      "cppStandard": "c++17",
      "intelliSenseMode": "macos-clang-x64"
    }
  ],
  "version": 4
}
```

## GTest

Ref: <https://github.com/bast/gtest-demo>

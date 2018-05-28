xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 98;
 0.00000;0.30211;-0.01322;,
 0.11942;0.26462;-0.14650;,
 0.00000;0.26462;-0.20170;,
 0.00000;0.30211;-0.01322;,
 0.16889;0.26462;-0.01322;,
 0.00000;0.30211;-0.01322;,
 0.11942;0.26462;0.12005;,
 0.00000;0.30211;-0.01322;,
 0.00000;0.26462;0.17525;,
 0.00000;0.30211;-0.01322;,
 -0.11942;0.26462;0.12005;,
 0.00000;0.30211;-0.01322;,
 -0.16889;0.26462;-0.01322;,
 0.00000;0.30211;-0.01322;,
 -0.11942;0.26462;-0.14650;,
 0.00000;0.30211;-0.01322;,
 0.00000;0.26462;-0.20170;,
 0.22067;0.15785;-0.25948;,
 0.00000;0.15785;-0.36148;,
 0.31207;0.15785;-0.01322;,
 0.22067;0.15785;0.23303;,
 0.00000;0.15785;0.33504;,
 -0.22067;0.15785;0.23303;,
 -0.31207;0.15785;-0.01322;,
 -0.22067;0.15785;-0.25948;,
 0.00000;0.15785;-0.36148;,
 0.28831;-0.00193;-0.33497;,
 0.00000;-0.00193;-0.46825;,
 0.40774;-0.00193;-0.01322;,
 0.28831;-0.00193;0.30853;,
 0.00000;-0.00193;0.44180;,
 -0.28831;-0.00193;0.30853;,
 -0.40774;-0.00193;-0.01322;,
 -0.28831;-0.00193;-0.33497;,
 0.00000;-0.00193;-0.46825;,
 0.31207;-0.19041;-0.58704;,
 0.00000;-0.19041;-0.73129;,
 0.44133;-0.19041;-0.01322;,
 0.31207;-0.19041;0.33504;,
 0.00000;-0.19041;0.47929;,
 -0.31207;-0.19041;0.33504;,
 -0.44133;-0.19041;-0.01322;,
 -0.31207;-0.19041;-0.58704;,
 0.00000;-0.19041;-0.73129;,
 0.28831;-0.37889;-0.92598;,
 0.00000;-0.37889;-1.05926;,
 0.40774;-0.37889;-0.01322;,
 0.28831;-0.37889;0.30853;,
 0.00000;-0.37889;0.44180;,
 -0.28831;-0.37889;0.30853;,
 -0.40774;-0.37889;-0.01322;,
 -0.28831;-0.37889;-0.92598;,
 0.00000;-0.37889;-1.05926;,
 0.22067;-0.53867;-1.08269;,
 0.00000;-0.53867;-1.18469;,
 0.31207;-0.53867;-0.01322;,
 0.22067;-0.53867;0.23303;,
 0.00000;-0.53867;0.33504;,
 -0.22067;-0.53867;0.23303;,
 -0.31207;-0.53867;-0.01322;,
 -0.22067;-0.53867;-1.08269;,
 0.00000;-0.53867;-1.18469;,
 -0.11942;0.26462;-0.14650;,
 -0.16889;0.26462;-0.01322;,
 -0.11942;0.26462;0.12005;,
 0.11942;0.26462;0.12005;,
 0.16889;0.26462;-0.01322;,
 0.11942;0.26462;-0.14650;,
 -0.22067;0.15785;-0.25948;,
 -0.31207;0.15785;-0.01322;,
 -0.22067;0.15785;0.23303;,
 0.22067;0.15785;0.23303;,
 0.31207;0.15785;-0.01322;,
 0.22067;0.15785;-0.25948;,
 -0.28831;-0.00193;-0.33497;,
 -0.40774;-0.00193;-0.01322;,
 -0.28831;-0.00193;0.30853;,
 0.28831;-0.00193;0.30853;,
 0.40774;-0.00193;-0.01322;,
 0.28831;-0.00193;-0.33497;,
 -0.31207;-0.19041;-0.58704;,
 -0.44133;-0.19041;-0.01322;,
 -0.31207;-0.19041;0.33504;,
 0.31207;-0.19041;0.33504;,
 0.44133;-0.19041;-0.01322;,
 0.31207;-0.19041;-0.58704;,
 -0.28831;-0.37889;-0.92598;,
 -0.40774;-0.37889;-0.01322;,
 -0.28831;-0.37889;0.30853;,
 0.28831;-0.37889;0.30853;,
 0.40774;-0.37889;-0.01322;,
 0.28831;-0.37889;-0.92598;,
 -0.22067;-0.53867;-1.08269;,
 -0.31207;-0.53867;-0.01322;,
 -0.22067;-0.53867;0.23303;,
 0.22067;-0.53867;0.23303;,
 0.31207;-0.53867;-0.01322;,
 0.22067;-0.53867;-1.08269;;
 
 108;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;27,26,35,36;,
 4;26,28,37,35;,
 4;28,29,38,37;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;36,35,44,45;,
 4;35,37,46,44;,
 4;37,38,47,46;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;45,44,53,54;,
 4;44,46,55,53;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;53,54,60,59;,
 4;59,60,61,53;,
 4;58,57,56,55;,
 4;55,56,57,58;,
 4;53,59,58,55;,
 4;55,58,59,53;,
 3;2,62,0;,
 3;62,63,3;,
 3;63,64,5;,
 3;64,8,7;,
 3;8,65,9;,
 3;65,66,11;,
 3;66,67,13;,
 3;67,16,15;,
 4;18,68,62,2;,
 4;68,69,63,62;,
 4;69,70,64,63;,
 4;70,21,8,64;,
 4;21,71,65,8;,
 4;71,72,66,65;,
 4;72,73,67,66;,
 4;73,25,16,67;,
 4;27,74,68,18;,
 4;74,75,69,68;,
 4;75,76,70,69;,
 4;76,30,21,70;,
 4;30,77,71,21;,
 4;77,78,72,71;,
 4;78,79,73,72;,
 4;79,34,25,73;,
 4;36,80,74,27;,
 4;80,81,75,74;,
 4;81,82,76,75;,
 4;82,39,30,76;,
 4;39,83,77,30;,
 4;83,84,78,77;,
 4;84,85,79,78;,
 4;85,43,34,79;,
 4;45,86,80,36;,
 4;86,87,81,80;,
 4;87,88,82,81;,
 4;88,48,39,82;,
 4;48,89,83,39;,
 4;89,90,84,83;,
 4;90,91,85,84;,
 4;91,52,43,85;,
 4;54,92,86,45;,
 4;92,93,87,86;,
 4;93,94,88,87;,
 4;94,57,48,88;,
 4;57,95,89,48;,
 4;95,96,90,89;,
 4;96,97,91,90;,
 4;97,61,52,91;,
 4;96,97,54,92;,
 4;92,61,97,96;,
 4;93,94,57,95;,
 4;95,57,94,93;,
 4;93,95,96,92;,
 4;92,96,95,93;;
 
 MeshMaterialList {
  1;
  108;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.458400;0.620800;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  95;
  0.000000;1.000000;0.000000;,
  0.000000;0.924937;-0.380120;,
  0.295139;0.917423;-0.266884;,
  0.414462;0.910066;0.000000;,
  0.295138;0.917423;0.266885;,
  -0.000000;0.924937;0.380120;,
  -0.295139;0.917423;0.266884;,
  -0.414463;0.910066;0.000000;,
  -0.295139;0.917423;-0.266884;,
  0.000000;0.709701;-0.704503;,
  0.534187;0.690415;-0.487823;,
  0.740364;0.672207;0.000000;,
  0.534186;0.690416;0.487823;,
  0.000000;0.709702;0.704502;,
  -0.534187;0.690415;0.487823;,
  -0.740365;0.672205;0.000000;,
  -0.534188;0.690414;-0.487823;,
  0.000000;0.697670;-0.716419;,
  0.641043;0.563089;-0.521531;,
  0.925655;0.377939;0.018036;,
  0.683997;0.367473;0.630168;,
  0.000000;0.384941;0.922941;,
  -0.683997;0.367474;0.630167;,
  -0.925654;0.377941;0.018036;,
  -0.641043;0.563089;-0.521530;,
  0.000000;0.841125;-0.540841;,
  0.705513;0.553668;-0.442383;,
  0.995912;0.065005;0.062713;,
  0.734317;-0.000000;0.678807;,
  0.000000;-0.000000;1.000000;,
  -0.734317;-0.000000;0.678806;,
  -0.995912;0.065004;0.062712;,
  -0.705514;0.553668;-0.442383;,
  0.000000;0.758423;-0.651763;,
  0.288470;0.726182;-0.624055;,
  0.952053;-0.289658;0.098455;,
  0.683997;-0.367473;0.630168;,
  0.000000;-0.384941;0.922941;,
  -0.683997;-0.367474;0.630167;,
  -0.952052;-0.289659;0.098455;,
  -0.963363;-0.229689;-0.138470;,
  0.000000;0.825283;-0.564719;,
  0.126289;0.953630;-0.273204;,
  0.879619;-0.463788;0.105694;,
  0.458511;-0.782743;0.420809;,
  0.000000;-0.795412;0.606069;,
  -0.345404;-0.883292;0.317002;,
  -0.879621;-0.463784;0.105694;,
  -0.903355;-0.417859;-0.096660;,
  -0.295139;0.917423;-0.266884;,
  -0.414462;0.910066;0.000000;,
  -0.295138;0.917423;0.266885;,
  0.295139;0.917423;0.266884;,
  0.414463;0.910066;0.000000;,
  0.295139;0.917423;-0.266884;,
  -0.534187;0.690415;-0.487823;,
  -0.740364;0.672207;0.000000;,
  -0.534186;0.690416;0.487823;,
  0.534187;0.690415;0.487823;,
  0.740365;0.672205;0.000000;,
  0.534188;0.690414;-0.487823;,
  -0.641043;0.563089;-0.521531;,
  -0.925655;0.377939;0.018036;,
  -0.683997;0.367473;0.630168;,
  0.683997;0.367474;0.630167;,
  0.925654;0.377941;0.018036;,
  0.641043;0.563089;-0.521530;,
  -0.705513;0.553668;-0.442383;,
  -0.995912;0.065005;0.062713;,
  -0.734317;-0.000000;0.678807;,
  0.734317;-0.000000;0.678806;,
  0.995912;0.065004;0.062712;,
  0.705514;0.553668;-0.442383;,
  -0.288470;0.726182;-0.624055;,
  -0.952053;-0.289658;0.098455;,
  -0.683997;-0.367473;0.630168;,
  0.683997;-0.367474;0.630167;,
  0.952052;-0.289659;0.098455;,
  0.963363;-0.229689;-0.138470;,
  -0.126289;0.953630;-0.273204;,
  -0.879619;-0.463788;0.105694;,
  -0.458511;-0.782743;0.420809;,
  0.345404;-0.883292;0.317002;,
  0.879621;-0.463784;0.105694;,
  0.903355;-0.417859;-0.096660;,
  0.963364;-0.229687;-0.138470;,
  -0.288472;0.726181;-0.624055;,
  0.903353;-0.417863;-0.096659;,
  -0.192210;0.888909;-0.415809;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.963364;-0.229687;-0.138470;,
  0.288472;0.726181;-0.624055;,
  -0.903353;-0.417863;-0.096659;,
  0.192210;0.888909;-0.415809;;
  108;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,85;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,86;,
  4;33,34,42,41;,
  4;85,35,43,87;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;86,33,41,88;,
  4;42,41,88,89;,
  4;90,90,90,90;,
  4;89,89,89,89;,
  4;90,44,45,46;,
  4;42,89,89,89;,
  4;90,46,90,90;,
  3;1,49,0;,
  3;49,50,0;,
  3;50,51,0;,
  3;51,5,0;,
  3;5,52,0;,
  3;52,53,0;,
  3;53,54,0;,
  3;54,1,0;,
  4;9,55,49,1;,
  4;55,56,50,49;,
  4;56,57,51,50;,
  4;57,13,5,51;,
  4;13,58,52,5;,
  4;58,59,53,52;,
  4;59,60,54,53;,
  4;60,9,1,54;,
  4;17,61,55,9;,
  4;61,62,56,55;,
  4;62,63,57,56;,
  4;63,21,13,57;,
  4;21,64,58,13;,
  4;64,65,59,58;,
  4;65,66,60,59;,
  4;66,17,9,60;,
  4;25,67,61,17;,
  4;67,68,62,61;,
  4;68,69,63,62;,
  4;69,29,21,63;,
  4;29,70,64,21;,
  4;70,71,65,64;,
  4;71,72,66,65;,
  4;72,25,17,66;,
  4;33,73,67,25;,
  4;91,74,68,67;,
  4;74,75,69,68;,
  4;75,37,29,69;,
  4;37,76,70,29;,
  4;76,77,71,70;,
  4;77,78,72,71;,
  4;92,33,25,72;,
  4;41,79,73,33;,
  4;93,80,74,91;,
  4;80,81,75,74;,
  4;81,45,37,75;,
  4;45,82,76,37;,
  4;82,83,77,76;,
  4;83,84,78,77;,
  4;94,41,33,92;,
  4;89,94,41,79;,
  4;90,90,90,90;,
  4;89,89,89,89;,
  4;82,45,81,90;,
  4;89,89,89,79;,
  4;90,90,82,90;;
 }
 MeshTextureCoords {
  98;
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.375000;0.125000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  0.125000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.125000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  0.125000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  0.125000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  0.125000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;;
 }
}

#ifndef __MODEL_LOADER_H__
#define __MODEL_LOADER_H__

/*
 * Face flags
 */
typedef enum
{
    Tmf_Face_None = 0,
    Tmf_Face_Double = 1,
    Tmf_Face_Meshed = 2
} TmfFaceFlags;

/*
 * Model file type
 */
typedef enum
{
    Tmf_Type_Static = 0
} TmfType;

/*
 * Texture data (image name, diffuse color)
 */
typedef struct
{
    char FileName[13];
    unsigned char color[3];
} TmfTexture;

/*
 * Quad data
 */
typedef struct
{
    POINT Normal;
    unsigned short Indexes[4];
    unsigned char Flags;
    unsigned char Texture;
    unsigned char Reserved[2];
} TmfFace;

/*
 * 3D model entry
 */
typedef struct
{
    unsigned short VerticesCount;
    unsigned short FaceCount;
} TmfModelHeader;

/*
 * Tank model file header
 */
typedef struct
{
    unsigned char Type;
    unsigned char TextureCount;
    unsigned char ModelCount;
    unsigned char Reserved[5];
} TmfHeader;

/*
 * Load Jo-Engine mesh (textures are loaded from the same folder model is in)
 * @param file File name inside models folder
 * @param dir Model file folder
 * @param loaded number of loaded models
 * @return loaded array of meshes
 */
jo_3d_mesh * ML_LoadMesh(const char *file, const char * dir, int *loaded);

/*
 * Load Jo-Engine mesh
 * @param file File name inside models folder
 * @param dir Model file folder
 * @param texture_loader texture loader
 * @param loaded number of loaded models
 * @return loaded array of meshes
 */
jo_3d_mesh * ML_LoadMeshWithCustomTextureLoader(const char * file, const char * dir, int (*texture_loader)(TmfTexture*, const char *, int), int * loaded);

#endif
#ifndef __EMIT_H__
#define __EMIT_H__

/**
 * Basic emited sprite
 */
typedef struct
{
    int SpriteId;
    int FramesAlive;
    int FramesToLive;
    jo_pos3D_fixed Location;
    jo_vector_fixed Velocity;
    jo_fixed BaseScaleDiv;
} Emit_data;

#endif
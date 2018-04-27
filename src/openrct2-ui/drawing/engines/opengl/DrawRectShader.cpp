#pragma region Copyright (c) 2014-2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#ifndef DISABLE_OPENGL

#include "DrawRectShader.h"

namespace
{
    struct VDStruct
    {
        GLfloat mat[4][2];
        GLfloat vec[2];
    };
}

constexpr VDStruct VertexData[4] =
{
    { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
};

DrawRectShader::DrawRectShader() : OpenGLShaderProgram("drawrect")
{
    GetLocations();

    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_vboInstances);
    glGenVertexArrays(1, &_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

    glBindVertexArray(_vao);

    glVertexAttribPointer(vVertMat+0, 2, GL_FLOAT, GL_FALSE, sizeof(VDStruct), (void*) offsetof(VDStruct, mat[0]));
    glVertexAttribPointer(vVertMat+1, 2, GL_FLOAT, GL_FALSE, sizeof(VDStruct), (void*) offsetof(VDStruct, mat[1]));
    glVertexAttribPointer(vVertMat+2, 2, GL_FLOAT, GL_FALSE, sizeof(VDStruct), (void*) offsetof(VDStruct, mat[2]));
    glVertexAttribPointer(vVertMat+3, 2, GL_FLOAT, GL_FALSE, sizeof(VDStruct), (void*) offsetof(VDStruct, mat[3]));
    glVertexAttribPointer(vVertVec,   2, GL_FLOAT, GL_FALSE, sizeof(VDStruct), (void*) offsetof(VDStruct, vec)   );

    glBindBuffer(GL_ARRAY_BUFFER, _vboInstances);
    glVertexAttribIPointer(vClip, 4, GL_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, clip));
    glVertexAttribIPointer(vTexColourAtlas, 1, GL_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, texColourAtlas));
    glVertexAttribPointer(vTexColourBounds, 4, GL_FLOAT, GL_FALSE, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, texColourBounds));
    glVertexAttribIPointer(vTexMaskAtlas, 1, GL_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, texMaskAtlas));
    glVertexAttribPointer(vTexMaskBounds, 4, GL_FLOAT, GL_FALSE, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, texMaskBounds));
    glVertexAttribIPointer(vPalettes, 3, GL_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, palettes));
    glVertexAttribIPointer(vFlags, 1, GL_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, flags));
    glVertexAttribIPointer(vColour, 1, GL_UNSIGNED_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, colour));
    glVertexAttribIPointer(vBounds, 4, GL_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, bounds));
    glVertexAttribIPointer(vDepth, 1, GL_INT, sizeof(DrawRectCommand), (void*) offsetof(DrawRectCommand, depth));

    glEnableVertexAttribArray(vVertMat+0);
    glEnableVertexAttribArray(vVertMat+1);
    glEnableVertexAttribArray(vVertMat+2);
    glEnableVertexAttribArray(vVertMat+3);
    glEnableVertexAttribArray(vVertVec);

    glEnableVertexAttribArray(vClip);
    glEnableVertexAttribArray(vTexColourAtlas);
    glEnableVertexAttribArray(vTexColourBounds);
    glEnableVertexAttribArray(vTexMaskAtlas);
    glEnableVertexAttribArray(vTexMaskBounds);
    glEnableVertexAttribArray(vPalettes);
    glEnableVertexAttribArray(vFlags);
    glEnableVertexAttribArray(vColour);
    glEnableVertexAttribArray(vBounds);
    glEnableVertexAttribArray(vDepth);

    glVertexAttribDivisor(vClip, 1);
    glVertexAttribDivisor(vTexColourAtlas, 1);
    glVertexAttribDivisor(vTexColourBounds, 1);
    glVertexAttribDivisor(vTexMaskAtlas, 1);
    glVertexAttribDivisor(vTexMaskBounds, 1);
    glVertexAttribDivisor(vPalettes, 1);
    glVertexAttribDivisor(vFlags, 1);
    glVertexAttribDivisor(vColour, 1);
    glVertexAttribDivisor(vBounds, 1);
    glVertexAttribDivisor(vDepth, 1);

    Use();
    glUniform1i(uTexture, 0);
    glUniform1i(uPaletteTex, 1);

    glUniform1i(uPeelingTex, 2);
    glUniform1i(uPeeling, 0);
}

DrawRectShader::~DrawRectShader()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_vboInstances);
    glDeleteVertexArrays(1, &_vao);
}

void DrawRectShader::GetLocations()
{
    uScreenSize         = GetUniformLocation("uScreenSize");
    uTexture            = GetUniformLocation("uTexture");
    uPaletteTex         = GetUniformLocation("uPaletteTex");

    uPeelingTex         = GetUniformLocation("uPeelingTex");
    uPeeling            = GetUniformLocation("uPeeling");

    vClip               = GetAttributeLocation("vClip");
    vTexColourAtlas     = GetAttributeLocation("vTexColourAtlas");
    vTexColourBounds    = GetAttributeLocation("vTexColourBounds");
    vTexMaskAtlas       = GetAttributeLocation("vTexMaskAtlas");
    vTexMaskBounds      = GetAttributeLocation("vTexMaskBounds");
    vPalettes           = GetAttributeLocation("vPalettes");
    vFlags              = GetAttributeLocation("vFlags");
    vColour             = GetAttributeLocation("vColour");
    vBounds             = GetAttributeLocation("vBounds");
    vDepth              = GetAttributeLocation("vDepth");

    vVertMat            = GetAttributeLocation("vVertMat");
    vVertVec            = GetAttributeLocation("vVertVec");
}

void DrawRectShader::SetScreenSize(sint32 width, sint32 height)
{
    glUniform2i(uScreenSize, width, height);
}

void DrawRectShader::EnablePeeling(GLuint peelingTex)
{
    OpenGLAPI::SetTexture(2, GL_TEXTURE_2D, peelingTex);
    glUniform1i(uPeeling, 1);
}

void DrawRectShader::DisablePeeling()
{
    glUniform1i(uPeeling, 0);
}

void DrawRectShader::SetInstances(const RectCommandBatch &instances)
{
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vboInstances);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DrawRectCommand) * instances.size(), instances.data(), GL_STREAM_DRAW);

    _instanceCount = (GLsizei)instances.size();
}

void DrawRectShader::DrawInstances()
{
    glBindVertexArray(_vao);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _instanceCount);
}

#endif /* DISABLE_OPENGL */

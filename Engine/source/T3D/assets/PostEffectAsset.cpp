//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef POSTEFFECT_ASSET_H
#include "PostEffectAsset.h"
#endif

#ifndef _ASSET_MANAGER_H_
#include "assets/assetManager.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

#ifndef _TAML_
#include "persistence/taml/taml.h"
#endif

#ifndef _ASSET_PTR_H_
#include "assets/assetPtr.h"
#endif

// Debug Profiling.
#include "platform/profiler.h"

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(PostEffectAsset);

ConsoleType(PostEffectAssetPtr, TypePostEffectAssetPtr, PostEffectAsset, ASSET_ID_FIELD_PREFIX)

//-----------------------------------------------------------------------------

ConsoleGetType(TypePostEffectAssetPtr)
{
   // Fetch asset Id.
   return (*((AssetPtr<PostEffectAsset>*)dptr)).getAssetId();
}

//-----------------------------------------------------------------------------

ConsoleSetType(TypePostEffectAssetPtr)
{
   // Was a single argument specified?
   if (argc == 1)
   {
      // Yes, so fetch field value.
      const char* pFieldValue = argv[0];

      // Fetch asset pointer.
      AssetPtr<PostEffectAsset>* pAssetPtr = dynamic_cast<AssetPtr<PostEffectAsset>*>((AssetPtrBase*)(dptr));

      // Is the asset pointer the correct type?
      if (pAssetPtr == NULL)
      {
         // No, so fail.
         //Con::warnf("(TypePostEffectAssetPtr) - Failed to set asset Id '%d'.", pFieldValue);
         return;
      }

      // Set asset.
      pAssetPtr->setAssetId(pFieldValue);

      return;
   }

   // Warn.
   Con::warnf("(TypePostEffectAssetPtr) - Cannot set multiple args to a single asset.");
}

//-----------------------------------------------------------------------------

PostEffectAsset::PostEffectAsset()
{
   mScriptFile = StringTable->EmptyString();
   mHLSLShaderFile = StringTable->EmptyString();
   mGLSLShaderFile = StringTable->EmptyString();

   mScriptPath = StringTable->EmptyString();
   mHLSLShaderPath = StringTable->EmptyString();
   mGLSLShaderPath = StringTable->EmptyString();
}

//-----------------------------------------------------------------------------

PostEffectAsset::~PostEffectAsset()
{
}

//-----------------------------------------------------------------------------

void PostEffectAsset::initPersistFields()
{
   docsURL;
   // Call parent.
   Parent::initPersistFields();

   addProtectedField("scriptFile", TypeAssetLooseFilePath, Offset(mScriptFile, PostEffectAsset),
      &setScriptFile, &getScriptFile, "Path to the script file.");
   addProtectedField("hlslShader", TypeAssetLooseFilePath, Offset(mHLSLShaderFile, PostEffectAsset),
      &setHLSLShaderFile, &getHLSLShaderFile, "Path to the hlsl shader file.");
   addProtectedField("glslShader", TypeAssetLooseFilePath, Offset(mGLSLShaderFile, PostEffectAsset),
      &setGLSLShaderFile, &getGLSLShaderFile, "Path to the glsl shader file.");
}

//------------------------------------------------------------------------------

void PostEffectAsset::copyTo(SimObject* object)
{
   // Call to parent.
   Parent::copyTo(object);
}

void PostEffectAsset::initializeAsset()
{
   mScriptPath = getOwned() ? expandAssetFilePath(mScriptFile) : mScriptPath;
   mHLSLShaderPath = getOwned() ? expandAssetFilePath(mHLSLShaderFile) : mHLSLShaderPath;
   mGLSLShaderPath = getOwned() ? expandAssetFilePath(mGLSLShaderFile) : mGLSLShaderPath;

   if (Torque::FS::IsScriptFile(mScriptPath))
      Con::executeFile(mScriptPath, false, false);
}

void PostEffectAsset::onAssetRefresh()
{
   mScriptPath = getOwned() ? expandAssetFilePath(mScriptFile) : mScriptPath;
   mHLSLShaderPath = getOwned() ? expandAssetFilePath(mHLSLShaderFile) : mHLSLShaderPath;
   mGLSLShaderPath = getOwned() ? expandAssetFilePath(mGLSLShaderFile) : mGLSLShaderPath;

   if (Torque::FS::IsScriptFile(mScriptPath))
      Con::executeFile(mScriptPath, false, false);
}

void PostEffectAsset::setScriptFile(const char* pScriptFile)
{
   // Sanity!
   AssertFatal(pScriptFile != NULL, "Cannot use a NULL script file.");

   // Fetch image file.
   pScriptFile = StringTable->insert(pScriptFile, true);

   // Ignore no change,
   if (pScriptFile == mScriptFile)
      return;

   // Update.
   mScriptFile = getOwned() ? expandAssetFilePath(pScriptFile) : pScriptFile;

   // Refresh the asset.
   refreshAsset();
}

void PostEffectAsset::setHLSLShaderFile(const char* pShaderFile)
{
   // Sanity!
   AssertFatal(pShaderFile != NULL, "Cannot use a NULL shader file.");

   // Fetch image file.
   pShaderFile = StringTable->insert(pShaderFile, true);

   // Ignore no change,
   if (pShaderFile == mHLSLShaderFile)
      return;

   // Update.
   mHLSLShaderFile = getOwned() ? expandAssetFilePath(pShaderFile) : pShaderFile;

   // Refresh the asset.
   refreshAsset();
}

void PostEffectAsset::setGLSLShaderFile(const char* pShaderFile)
{
   // Sanity!
   AssertFatal(pShaderFile != NULL, "Cannot use a NULL shader file.");

   // Fetch image file.
   pShaderFile = StringTable->insert(pShaderFile, true);

   // Ignore no change,
   if (pShaderFile == mGLSLShaderFile)
      return;

   // Update.
   mGLSLShaderFile = getOwned() ? expandAssetFilePath(pShaderFile) : pShaderFile;

   // Refresh the asset.
   refreshAsset();
}

DefineEngineMethod(PostEffectAsset, getScriptPath, const char*, (), ,
   "Gets the script file path for the asset.")
{
   return object->getScriptPath();
}

DefineEngineMethod(PostEffectAsset, getHLSLShaderPath, const char*, (), ,
   "Gets the HLSL Shader file path for the asset.")
{
   return object->getHLSLShaderPath();
}

DefineEngineMethod(PostEffectAsset, getGLSLShaderPath, const char*, (), ,
   "Gets the GLSL Shader file path for the asset.")
{
   return object->getGLSLShaderPath();
}

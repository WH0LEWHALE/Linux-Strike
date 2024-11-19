//====== Copyright  Valve Corporation, All rights reserved. =================
//
// Purpose:
//
//=============================================================================

#include "cbase.h"
#include "cs_custom_texture_saver.h"

bool CCSCustomTextureSaver::Init()
{
	m_materials.RemoveAll();
	m_bHasJob = false;
	return true;
}

void CCSCustomTextureSaver::AddMaterialToWatch( CustomMaterialGenerationData_t embroiderMaterial )
{
	m_materials.AddToTail( embroiderMaterial );
	m_bHasJob = true;
}

void CCSCustomTextureSaver::Update( float frameTime )
{
	if ( m_bHasJob == false )
		return;

	int count = m_materials.Count();
	
	if ( count == 0 )
	{
		m_bHasJob = false;
		return;
	}
	
	CUtlVector<CustomMaterialGenerationData_t> completedMaterials;

	for ( int i = 0; i < count; ++i )
	{
		int completionCount = 0;

		for ( int j = 0; j < m_materials[i].nTex; j++ )
		{
			completionCount += ( m_materials[i].pCustomMaterial->GetTexture( j )->GenerationComplete() );
		}
		if ( completionCount == 3 )
		{
			count--;
			completedMaterials.AddToTail( m_materials[i] );
			m_materials.Remove( i );
			i--;
		}
	}

	for ( int i = 0; i < completedMaterials.Count(); i++ )
	{
		for ( int j = 0; j < completedMaterials[i].nTex; j++ )
		{
			if ( g_pFullFileSystem->FileExists( completedMaterials[i].fileNames[j], "MOD" ) &&  !g_pFullFileSystem->IsFileWritable( completedMaterials[i].fileNames[j], "MOD" ) )
			{
				// Skip read only files
				continue;
			}

			IVTFTexture *pVTFTexture = completedMaterials[i].pCustomMaterial->GetTexture( j )->GetResultVTF();

			CUtlBuffer buf;
			pVTFTexture->Serialize(buf);
					
			FileHandle_t f = g_pFullFileSystem->Open( completedMaterials[i].fileNames[j], "wb", NULL );

			if ( f != FILESYSTEM_INVALID_HANDLE )
			{
				g_pFullFileSystem->Write( buf.Base(), buf.TellMaxPut(), f );
				g_pFullFileSystem->Close(f);
			}
			DevMsg( "Saved VTF %s\n", completedMaterials[i].fileNames[j] );

			char szFullVTFPath[ MAX_PATH ];
			g_pFullFileSystem->RelativePathToFullPath( completedMaterials[i].fileNames[j], "MOD", szFullVTFPath, sizeof( szFullVTFPath ) );
		}
		// TODO: add saved files to perforce
	}
}



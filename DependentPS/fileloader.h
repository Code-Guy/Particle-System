#ifndef _FILELOADER_H
#define  _FILELOADER_H

#include <string>
#include "mesh.h"
#include "particlesystem.h"

#include "libraries/stbi/stb_image.h"

const std::string BaseModelPath = "media/models/";
const std::string BaseTexturePath = "media/textures/";
const std::string BaseShaderPath = "media/shaders/";
const std::string BaseParamPath = "media/params/";

const ork::Texture::Parameters DefaultTexturePara = ork::Texture::Parameters().min(ork::TextureFilter::LINEAR).mag(ork::TextureFilter::LINEAR).wrapS(ork::TextureWrap::CLAMP_TO_EDGE).wrapT(ork::TextureWrap::CLAMP_TO_EDGE);

class FileLoader
{
public:
	//�����ļ��������ļ����ݣ�char *�����ļ���С
	static unsigned char* LoadFile(const std::string filePath, int &size);
	//�����ļ��������ļ����ݣ�char *��
	static unsigned char* LoadFile(const std::string filePath);

	//����program
	static ork::ptr<ork::Program> LoadProgram(const std::string filePath);
	//������պ�
	static ork::ptr<ork::TextureCube> LoadTextureCube(const std::string filePath, ork::Texture::Parameters para = DefaultTexturePara);
	//������ͼ
	static ork::ptr<ork::Texture2D> LoadTexture(const std::string filePath, ork::Texture::Parameters para = DefaultTexturePara);
	//����obj mesh�ļ�
	static bool LoadObjMesh(const std::string filePath, std::vector<ObjMesh> &meshes, std::string &mtlName);
	//����ork mesh�ļ�
	static bool LoadOrkMesh(const std::string filePath, std::vector<OrkMesh> &meshes);
	//��������ϵͳ�����ļ�
	static bool LoadParticleSystem(const string fileName, ParticleSystem *ps);

	//��ȡ��Դ·��
	friend std::string GetModelPath(std::string fileName);
	friend std::string GetTexturePath(std::string fileName);
	friend std::string GetShaderPath(std::string fileName);
	friend std::string GetParamPath(std::string fileName);
	//�ַ����ָ��
	friend void Split(std::string& s, std::string& delim, std::vector< std::string >* ret);
	//ȥ���ַ�������λ�ո�
	friend std::string Trim(std::string s);

private:
	//������ʺ���ͼ
	static bool LoadMaterialTex(const std::string mtlName, std::vector<ObjMesh> &objMeshes);
	//���Զ����objmesh�����ork mesh
	static void FillMeshes(std::vector<ObjMesh> &objMeshes, std::vector<OrkMesh> &meshes);
};

#endif //_FILELOADER_H
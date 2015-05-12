#include "fileloader.h"
#include "math3d.h"

#include <cstdio>
#include <ctime>

using namespace std;
using namespace ork;

//加载文件，返回文件内容（char *）和文件大小
unsigned char* FileLoader::LoadFile(const string filePath, int &size)
{
	FILE *file = fopen(filePath.c_str(), "rb");

	if (file == NULL)//打开文件错误，例如文件路径不存在，直接返回NULL
	{
		return NULL;
	}

	//先获取文件大小
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	//回到文件头
	fseek(file, 0, SEEK_SET);

	char *data = new char[size + 1];
	fread(data, size, 1, file);
	data[size] = 0;//最后一个字符为0？\0

	fclose(file);

	return (unsigned char *)data;
}

//加载文件，返回文件内容（char *）
unsigned char* FileLoader::LoadFile(const string filePath)
{
	FILE *file = fopen(filePath.c_str(), "rb");

	if (file == NULL)//打开文件错误，例如文件路径不存在，直接返回NULL
	{
		return NULL;
	}

	//先获取文件大小
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	//回到文件头
	fseek(file, 0, SEEK_SET);

	char *data = new char[size + 1];
	fread(data, size, 1, file);
	data[size] = 0;//最后一个字符为0？\0

	fclose(file);

	return (unsigned char *)data;
}

//加载program
ptr<Program> FileLoader::LoadProgram(const string filePath)
{
	return new Program(new Module(330, (char *)LoadFile(filePath)));
}

//加载天空盒
ptr<TextureCube> FileLoader::LoadTextureCube(const string filePath, Texture::Parameters para)
{
	ptr<Buffer> buffers[6];
	Buffer::Parameters texpar[6] = { Buffer::Parameters() };
	int size, w, h, channels;
	string curpath = filePath + "/posx.jpg";
	unsigned char* chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	unsigned char* tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[0] = new CPUBuffer(tempdata);

	curpath = filePath + "/negx.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[1] = new CPUBuffer(tempdata);

	curpath = filePath + "/posy.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[2] = new CPUBuffer(tempdata);

	curpath = filePath + "/negy.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[3] = new CPUBuffer(tempdata);

	curpath = filePath + "/posz.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[4] = new CPUBuffer(tempdata);

	curpath = filePath + "/negz.jpg";
	chardata = LoadFile(curpath.c_str(), size);
	if (chardata == NULL)
	{
		return NULL;
	}
	tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);
	buffers[5] = new CPUBuffer(tempdata);

	if (channels == 3)
		return new TextureCube(w, h, RGB8, RGB, PixelType::UNSIGNED_BYTE, para, texpar, buffers);
	return new TextureCube(w, h, RGBA8, RGBA, PixelType::UNSIGNED_BYTE, para, texpar, buffers);
}

//加载贴图
ptr<Texture2D> FileLoader::LoadTexture(const string filePath, Texture::Parameters para)
{
	int w, h, channels, size;
	unsigned char* chardata = LoadFile(filePath, size);
	if (chardata == NULL)
	{
		return NULL;
	}
	unsigned char* tempdata = stbi_load_from_memory(chardata, size, &w, &h, &channels, 0);

	if (channels == 3)
		return new Texture2D(w, h, RGB8, RGB, UNSIGNED_BYTE, para, Buffer::Parameters(), CPUBuffer(tempdata));
	return new Texture2D(w, h, RGBA8, RGBA, UNSIGNED_BYTE, para, Buffer::Parameters(), CPUBuffer(tempdata));
}

//加载obj mesh文件
bool FileLoader::LoadObjMesh(const string filePath, vector<ObjMesh> &meshes, string &mtlName)
{
	printf("Loading mesh %s\n", filePath);

	meshes.clear();
	ObjMesh objMesh;//一个obj mesh

	bool isNextMesh = false;

	int vOffset = VBO::vertices.size();
	int uvOffset = VBO::uvs.size();
	int nOffset = VBO::normals.size();

	FILE *file = fopen(filePath.c_str(), "r");
	if (file == NULL)
	{
		printf("Could not open this obj file!\n");
		getchar();
		return false;
	}

	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "g") == 0)
		{
			fscanf(file, "%s\n", objMesh.ID);
			isNextMesh = true;
		}
		else if (strcmp(lineHeader, "mtllib") == 0)
		{
			char mtlNameStr[MAX_ID_LEN];//mtl文件名
			fscanf(file, "%s\n", mtlNameStr);
			mtlName = mtlNameStr;
		}
		else if (strcmp(lineHeader, "usemtl") == 0)
		{
			fscanf(file, "%s\n", objMesh.MID);
		}
		else if (strcmp(lineHeader, "v") == 0)
		{
			if (isNextMesh)
			{
				meshes.push_back(objMesh);
				objMesh.Clear();
				isNextMesh = false;
			}
			vec3f v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
			VBO::vertices.push_back(v);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vec2f uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			VBO::uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3f n;
			fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
			VBO::normals.push_back(n);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			//简单起见，这里的面只是三角面^_^
			Face face;
			int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			face.refers.push_back(Refer(vertexIndex[0] - 1 + vOffset, uvIndex[0] - 1 + uvOffset, normalIndex[0] - 1 + nOffset));
			face.refers.push_back(Refer(vertexIndex[1] - 1 + vOffset, uvIndex[1] - 1 + uvOffset, normalIndex[1] - 1 + nOffset));
			face.refers.push_back(Refer(vertexIndex[2] - 1 + vOffset, uvIndex[2] - 1 + uvOffset, normalIndex[2] - 1 + nOffset));

			objMesh.faces.push_back(face);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	fclose(file);

	meshes.push_back(objMesh);

	return true;
}

//加载ork mesh文件
bool FileLoader::LoadOrkMesh(const string filePath, vector<OrkMesh> &meshes)
{
	ObjMesh objMesh;//一个obj mesh
	vector<ObjMesh> objMeshes;//所有obj mesh
	string mtlName = "";

	LoadObjMesh(filePath, objMeshes, mtlName);

	if (mtlName != "")
		LoadMaterialTex(mtlName, objMeshes);//载入材质和贴图到objMeshes

	FillMeshes(objMeshes, meshes);//用自定义的objmesh来填充ork mesh

	VBO::Clear();//清空vbo

	return true;
}

//载入材质和贴图
bool FileLoader::LoadMaterialTex(const string mtlName, vector<ObjMesh> &objMeshes)
{
	printf("  %s\n", GetModelPath(mtlName));

	FILE *file = fopen(GetModelPath(mtlName).c_str(), "r");
	if (file == NULL)
	{
		getchar();
		return false;
	}

	vector<ObjMesh>::iterator mIter;//一个mesh迭代器

	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "newmtl") == 0)
		{
			char MID[MAX_ID_LEN];
			fscanf(file, "%s\n", MID);

			mIter = GetObjMeshWithMID(objMeshes, MID);
		}
		else if (strcmp(lineHeader, "Kd") == 0 && mIter != objMeshes.end())
		{
			fscanf(file, "%f %f %f\n", &(mIter->material.diffuseColor.x), &(mIter->material.diffuseColor.y), &(mIter->material.diffuseColor.z));
		}
		else if (strcmp(lineHeader, "Ka") == 0 && mIter != objMeshes.end())
		{
			fscanf(file, "%f %f %f\n", &(mIter->material.ambientColor.x), &(mIter->material.ambientColor.y), &(mIter->material.ambientColor.z));
		}
		else if (strcmp(lineHeader, "Ks") == 0 && mIter != objMeshes.end())
		{
			fscanf(file, "%f %f %f\n", &(mIter->material.specularColor.x), &(mIter->material.specularColor.y), &(mIter->material.specularColor.z));
		}
		else if (strcmp(lineHeader, "map_Kd") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->diffuseTexName = string(texName);
		}
		else if (strcmp(lineHeader, "map_Ka") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->ambientTexName = string(texName);
		}
		else if (strcmp(lineHeader, "map_Ks") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->specularTexName = string(texName);
		}
		else if (strcmp(lineHeader, "bump") == 0 && mIter != objMeshes.end())
		{
			char texName[MAX_ID_LEN];
			fscanf(file, "%s\n", texName);
			mIter->normalTexName = string(texName);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	return true;
}

//用自定义的objmesh来填充ork mesh
void FileLoader::FillMeshes(vector<ObjMesh> &objMeshes, vector<OrkMesh> &meshes)
{
	//遍历所有obj mesh
	for (vector<ObjMesh>::iterator oIter = objMeshes.begin(); oIter != objMeshes.end(); oIter++)
	{
		AABB aabb;
		aabb = oIter->CalcAABB();//计算包围盒

		ptr< Mesh<V_UV_N_T_B, unsigned int> > mesh = new Mesh<V_UV_N_T_B, unsigned int>(TRIANGLES, GPU_STATIC);
		mesh->addAttributeType(0, 3, A32F, false);
		mesh->addAttributeType(1, 2, A32F, false);
		mesh->addAttributeType(2, 3, A32F, false);
		mesh->addAttributeType(3, 2, A32F, false);
		mesh->addAttributeType(4, 3, A32F, false);

		//遍历obj mesh的所有face
		for (vector<Face>::const_iterator fIter = oIter->faces.begin(); fIter != oIter->faces.end(); fIter++)
		{
			vec3f T, B;
			CalcTB(T, B, *fIter);//计算Tangent和Bitangent向量
			for (vector<Refer>::const_iterator rIter = fIter->refers.begin(); rIter != fIter->refers.end(); rIter++)
			{
				mesh->addVertex(V_UV_N_T_B(VBO::vertices[rIter->v].x, VBO::vertices[rIter->v].y, VBO::vertices[rIter->v].z,
					VBO::uvs[rIter->uv].x, VBO::uvs[rIter->uv].y,
					VBO::normals[rIter->n].x, VBO::normals[rIter->n].y, VBO::normals[rIter->n].z,
					T.x, T.y, T.z, B.x, B.y, B.z));

			}
		}

		if (oIter->diffuseTexName != "")
		{
			printf("    Diffuse Texture: %s\n", oIter->diffuseTexName.c_str());
		}
		if (oIter->normalTexName != "")
		{
			printf("    Normal Texture: %s\n", oIter->normalTexName.c_str());
		}
		if (oIter->specularTexName != "")
		{
			printf("    Specular Texture: %s\n", oIter->specularTexName.c_str());
		}
		if (oIter->ambientTexName != "")
		{
			printf("    Ambient Texture: %s\n", oIter->ambientTexName.c_str());
		}

		meshes.push_back(OrkMesh(mesh, oIter->material, aabb, 
			LoadTexture(GetTexturePath(oIter->diffuseTexName)),
			LoadTexture(GetTexturePath(oIter->normalTexName)),
			LoadTexture(GetTexturePath(oIter->specularTexName))));
	}
}

//加载粒子系统参数文件
bool FileLoader::LoadParticleSystem(const string fileName, ParticleSystem *ps)
{
	static int ID = 0;
	string filePath = GetParamPath(fileName);
	FILE* file = fopen(filePath.c_str(), "r");
	if (file == NULL){
		printf("Could not open this config file!\n");
		getchar();
		return false;
	}

	//粒子系统名称
	ps->name = fileName;
	//粒子系统ID
	ps->ID = ID++;

	//粒子系统发射器的transformation信息
	vec3f position;
	vec3f rotation;
	vec3f scale;

	while (true)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "EmitterType") == 0)
		{
			fscanf(file, "%d\n", &ps->emitterType);

			switch (ps->emitterType)
			{
			case 0:
				ps->emitter = new EllipsoidParticleEmitter();
				break;
			case 1:
				ps->emitter = new MeshParticleEmitter();
				break;
			default:
				ps->emitter = new EllipsoidParticleEmitter();
				break;
			}

			ps->emitter->position = position;
			ps->emitter->rotation = rotation;
			ps->emitter->scale = scale;
		}
		else if (strcmp(lineHeader, "Position:") == 0)
		{
			fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
		}
		else if (strcmp(lineHeader, "Rotation:") == 0)
		{
			fscanf(file, "%f %f %f\n", &rotation.x, &rotation.y, &rotation.z);
		}
		else if (strcmp(lineHeader, "LocalScale:") == 0)
		{
			fscanf(file, "%f %f %f\n", &scale.x, &scale.y, &scale.z);
		}
		else if (strcmp(lineHeader, "Emit") == 0)
		{
			int isEmit;
			fscanf(file, "%d\n", &isEmit);
			ps->emitter->isEmit = isEmit;
		}
		else if (strcmp(lineHeader, "Size") == 0)
		{
			fscanf(file, "%f %f\n", &ps->emitter->minSize, &ps->emitter->maxSize);
		}
		else if (strcmp(lineHeader, "Energy") == 0)
		{
			fscanf(file, "%f %f\n", &ps->emitter->minEnergy, &ps->emitter->maxEnergy);
		}
		else if (strcmp(lineHeader, "Emission") == 0)
		{
			fscanf(file, "%f %f\n", &ps->emitter->minEmission, &ps->emitter->maxEmission);
		}
		else if (strcmp(lineHeader, "WorldVelocity") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->emitter->worldVelocity.x, &ps->emitter->worldVelocity.y, &ps->emitter->worldVelocity.z);
		}
		else if (strcmp(lineHeader, "LocalVelocity") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->emitter->localVelocity.x, &ps->emitter->localVelocity.y, &ps->emitter->localVelocity.z);
		}
		else if (strcmp(lineHeader, "RndVelocity") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->emitter->rndVelocity.x, &ps->emitter->rndVelocity.y, &ps->emitter->rndVelocity.z);
		}
		else if (strcmp(lineHeader, "EmitterVelocityScale") == 0)
		{
			fscanf(file, "%f\n", &ps->emitter->emitterVelocityScale);
		}
		else if (strcmp(lineHeader, "TangentVelocity") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->emitter->tangentVelocity.x, &ps->emitter->tangentVelocity.y, &ps->emitter->tangentVelocity.z);
		}
		else if (strcmp(lineHeader, "AngularVelocity") == 0)
		{
			fscanf(file, "%f\n", &ps->emitter->angularVelocity);
		}
		else if (strcmp(lineHeader, "RndAngularVelocity") == 0)
		{
			fscanf(file, "%f\n", &ps->emitter->rndAngularVelocity);
		}
		else if (strcmp(lineHeader, "RndRotation") == 0)
		{
			int isRndRot;
			fscanf(file, "%d\n", &isRndRot);
			ps->emitter->rndRotation = isRndRot;
		}
		else if (strcmp(lineHeader, "SimulateInWorldspace") == 0)
		{
			int isSimulateInWorldspace;
			fscanf(file, "%d\n", &isSimulateInWorldspace);
			ps->emitter->isSimulateInWorldspace = isSimulateInWorldspace;
		}
		else if (strcmp(lineHeader, "OneShot") == 0)
		{
			int isOneShot;
			fscanf(file, "%d\n", &isOneShot);
			ps->emitter->isOneShot = isOneShot;
		}
		else if (strcmp(lineHeader, "Ellipsoid") == 0 && ps->emitterType == 0)
		{
			fscanf(file, "%f %f %f\n", &dynamic_cast<EllipsoidParticleEmitter *>(ps->emitter)->ellipoid.x,
				&dynamic_cast<EllipsoidParticleEmitter *>(ps->emitter)->ellipoid.y,
				&dynamic_cast<EllipsoidParticleEmitter *>(ps->emitter)->ellipoid.z);
		}
		else if (strcmp(lineHeader, "MinEmitterRange") == 0 && ps->emitterType == 0)
		{
			fscanf(file, "%f\n", &dynamic_cast<EllipsoidParticleEmitter *>(ps->emitter)->minEmitterRange);
		}
		else if (strcmp(lineHeader, "InterpolateTriangles") == 0 && ps->emitterType == 1)
		{
			int isInterpolateTris;
			fscanf(file, "%d\n", &isInterpolateTris);
			dynamic_cast<MeshParticleEmitter *>(ps->emitter)->isInterpolateTris = isInterpolateTris;
		}
		else if (strcmp(lineHeader, "Systematic") == 0 && ps->emitterType == 1)
		{
			int isSystematic;
			fscanf(file, "%d\n", &isSystematic);
			dynamic_cast<MeshParticleEmitter *>(ps->emitter)->isSystematic = isSystematic;
		}
		else if (strcmp(lineHeader, "NormalVelocity") == 0 && ps->emitterType == 1)
		{
			fscanf(file, "%f %f\n", &dynamic_cast<MeshParticleEmitter *>(ps->emitter)->minNormalVelocity, 
				&dynamic_cast<MeshParticleEmitter *>(ps->emitter)->maxNormalVelocity);
		}
		else if (strcmp(lineHeader, "MeshName") == 0 && ps->emitterType == 1)
		{
			fscanf(file, "%s\n", &dynamic_cast<MeshParticleEmitter *>(ps->emitter)->meshName);

			vector<ObjMesh> objMeshes;//所有obj mesh
			string mtlName = "";
			LoadObjMesh(GetModelPath("/EmitterMesh/" + string(dynamic_cast<MeshParticleEmitter *>(ps->emitter)->meshName) + ".obj"), 
				objMeshes, mtlName);

			dynamic_cast<MeshParticleEmitter *>(ps->emitter)->mesh = objMeshes.front();
		}
		else if (strcmp(lineHeader, "DoesAnimateColor") == 0)
		{
			int fIsAniCol;
			fscanf(file, "%d\n", &fIsAniCol);
			ps->animator->doesAnimateColor = fIsAniCol;
		}
		else if (strcmp(lineHeader, "AnimationColors") == 0)
		{
			for (int i = 0; i < 5; i++)
			{
				fscanf(file, "%f %f %f %f", &ps->animator->animationColor[i].x, &ps->animator->animationColor[i].y, &ps->animator->animationColor[i].z, &ps->animator->animationColor[i].w);
			}
		}
		else if (strcmp(lineHeader, "WorldRotationAxis") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->animator->worldRotationAxis.x, &ps->animator->worldRotationAxis.y, &ps->animator->worldRotationAxis.z);
		}
		else if (strcmp(lineHeader, "LocalRotationAxis") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->animator->localRotationAxis.x, &ps->animator->localRotationAxis.y, &ps->animator->localRotationAxis.z);
		}
		else if (strcmp(lineHeader, "SizeGrow") == 0)
		{
			fscanf(file, "%f\n", &ps->animator->sizeGrow);
		}
		else if (strcmp(lineHeader, "RndForce") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->animator->rndForce.x, &ps->animator->rndForce.y, &ps->animator->rndForce.z);
		}
		else if (strcmp(lineHeader, "Force") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->animator->force.x, &ps->animator->force.y, &ps->animator->force.z);
		}
		else if (strcmp(lineHeader, "Damping") == 0)
		{
			fscanf(file, "%f\n", &ps->animator->damping);
		}
		else if (strcmp(lineHeader, "Autodestruct") == 0)
		{
			int autodestruct;
			fscanf(file, "%d\n", &autodestruct);
			ps->animator->autoDestruct = autodestruct;
		}
		else if (strcmp(lineHeader, "CastShadows") == 0)
		{
			int castShadows;
			fscanf(file, "%d\n", &castShadows);
			ps->renderer->castShadows = castShadows;
		}
		else if (strcmp(lineHeader, "ReceiveShadows") == 0)
		{
			int receiveShadows;
			fscanf(file, "%d\n", &receiveShadows);
			ps->renderer->receiveShadows = receiveShadows;
		}
		else if (strcmp(lineHeader, "UseLightProbes") == 0)
		{
			int useLightProbes;
			fscanf(file, "%d\n", &useLightProbes);
			ps->renderer->receiveShadows = useLightProbes;
		}
		else if (strcmp(lineHeader, "LightProbeAnchor") == 0)
		{
			char lightProbeAnchor[MAX_ID_LEN];
			fscanf(file, "%s\n", &lightProbeAnchor);
			ps->renderer->lightProbeAnchor = lightProbeAnchor;
		}
		else if (strcmp(lineHeader, "CameraVelocityScale") == 0)
		{
			fscanf(file, "%f\n", &ps->renderer->cameraVelocityScale);
		}
		else if (strcmp(lineHeader, "StretchedParticles") == 0)
		{
			char billboardStyleName[MAX_ID_LEN];
			fscanf(file, "%s\n", billboardStyleName);
			ps->renderer->GetBillboardStyle(billboardStyleName);
		}
		else if (strcmp(lineHeader, "LengthScale") == 0)
		{
			fscanf(file, "%f\n", &ps->renderer->lengthScale_velocityScale.x);
		}
		else if (strcmp(lineHeader, "VelocityScale") == 0)
		{
			fscanf(file, "%f\n", &ps->renderer->lengthScale_velocityScale.y);
		}
		else if (strcmp(lineHeader, "MaxParticleSize") == 0)
		{
			fscanf(file, "%f\n", &ps->renderer->maxParticleSize);
		}
		else if (strcmp(lineHeader, "UVAnimation") == 0)
		{
			fscanf(file, "%f %f %f\n", &ps->renderer->uvAniamtion.x, &ps->renderer->uvAniamtion.y, &ps->renderer->uvAniamtion.z);
		}
		else if (strcmp(lineHeader, "TextureNum") == 0)
		{
			fscanf(file, "%d\n", &ps->renderer->texNum);
		}
		else if (strcmp(lineHeader, "TextureNames") == 0)
		{
			char rawTextureNames[MAX_STRING_LEN];
			fgets(rawTextureNames, MAX_STRING_LEN, file);

			vector<string> textureNames;
			Split(string(rawTextureNames), string(" "), &textureNames);

			for (size_t i = 0; i < textureNames.size(); i++)
			{
				string texPath = GetTexturePath(textureNames[i]);
				printf("%s\n", texPath.c_str());
				ps->renderer->texturePaths.push_back(texPath);
				ps->renderer->particleTex.push_back(LoadTexture(texPath));
			}

			ps->renderer->particleProgram->getUniformSampler("ParticleTex")->set(ps->renderer->particleTex[0]);
			ps->renderer->texSize = vec2f(ps->renderer->particleTex[0]->getWidth(), ps->renderer->particleTex[0]->getHeight());
		}
		else if (strcmp(lineHeader, "TintColor") == 0)
		{
			fscanf(file, "%f %f %f %f\n", &ps->animator->tintColor.x, &ps->animator->tintColor.y, &ps->animator->tintColor.z, &ps->animator->tintColor.w);
			if (Math3D::IsVec4Equal(ps->animator->tintColor, vec4f::ZERO))
			{
				ps->animator->tintColor = vec4f(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				ps->animator->tintColor = ps->animator->tintColor;
			}
		}
		else if (strcmp(lineHeader, "ProgramName") == 0)
		{
			char particleMatName[MAX_ID_LEN];
			fgets(particleMatName, MAX_ID_LEN, file);
			fscanf(file, "%s\n", particleMatName);
			particleMatName[strlen(particleMatName) - 1] = '\0';
			ps->renderer->GetParticleMaterialStyle(Trim(particleMatName));
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	fclose(file);

	return true;
}

//获取资源路径
string GetModelPath(string fileName)
{
	return BaseModelPath + fileName;
}

string GetTexturePath(string fileName)
{
	return BaseTexturePath + fileName;
}

string GetShaderPath(string fileName)
{
	return BaseShaderPath + fileName;
}

string GetParamPath(string fileName)
{
	return BaseParamPath + fileName;
}

//字符串分割函数
void Split(string& s, string& delim, vector< string >* ret)
{
	s = Trim(s);
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != string::npos)
	{
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		ret->push_back(Trim(s.substr(last, index - last)));
	}
}

string Trim(string s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	s.erase(s.find_last_not_of("\n") + 1);
	return s;
}

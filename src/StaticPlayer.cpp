#include "StaticPlayer.h"

void StaticPlayer::DefineVertices(string path)
{
	if (path == "no path") {

	}
	else {
		// load .obj
		// add texture Path;

		std::string sExecutableDirectory = Path_StripFilename(Path_GetExecutablePath());
		std::string strFullPath = Path_MakeAbsolute(string(path), sExecutableDirectory);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(strFullPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// retrieve the directory path of the filepath
		//directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);


	}
}

void StaticPlayer::Render(Matrix4 projectionview)
{
	glUseProgram(ShaderProgramID);

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (textures[i].id != 0) {
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding

			//switch (textures[i].type) {
			//case DiffuseMap:
			//	glUniform1i(glGetUniformLocation(ShaderProgramID, "diffuse"), i);
			//	cout << "set diffuse" << endl;
			//	break;
			//case MetallicMap:
			//	glUniform1i(glGetUniformLocation(ShaderProgramID, "metallic"), i);
			//	cout << "set metallic" << endl;
			//	break;
			//case NormalMap:
			//	glUniform1i(glGetUniformLocation(ShaderProgramID, "normal"), i);
			//	cout << "set normal" << endl;
			//	break;
			//case RoughnessMap:
			//	glUniform1i(glGetUniformLocation(ShaderProgramID, "rough"), i);
			//	cout << "set rough" << endl;
			//	break;
			//default:
			//	cout << "hmmm" << endl;
			//	break;
			//}
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// now set the sampler to the correct texture uni
		// and finally bind the texture

		//
	}

	//glUniformMatrix4fv(MatrixLocation, 1, GL_FALSE, projectionview.get());
	setMat4_Shader("Transform", TransformMatrix);
	glUniformMatrix4fv(MatrixLocation, 1, GL_FALSE, projectionview.get());
	//glUniformMatrix4fv(MatrixLocation, 1, GL_FALSE, TransformMatrix.get());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
	
}

bool StaticPlayer::bindtexture()
{
	cout << "+++++++++++++++++++++++++++++++ \n loading texture: " << endl;
	Texture diffuseMap = loadMaterialTextures(DiffuseMap);
	if (diffuseMap.id != 0) 
	{
		cout << "loaded diffuse" << endl;
		textures.push_back(diffuseMap);
	}
	// 2. specular maps
	Texture metallicMap = loadMaterialTextures(MetallicMap);
	if (metallicMap.id != 0)
	{
		cout << "loaded metallic" << endl;
		textures.push_back(metallicMap);
	}
	// 3. normal maps
	Texture normalMap = loadMaterialTextures(NormalMap);
	if (normalMap.id != 0)
	{
		cout << "loaded normal" << endl;
		textures.push_back(normalMap);
	}
	// 4. height maps
	Texture roughmap = loadMaterialTextures(RoughnessMap);
	if (roughmap.id != 0) 
	{
		cout << "loaded rough" << endl;
		textures.push_back(roughmap);
	}
	cout << " loaded texture \n+++++++++++++++++++++++++++++++ " << endl;
		

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;

	return true;

	// draw mesh
	

}

void StaticPlayer::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void StaticPlayer::processMesh(aiMesh* mesh, const aiScene* scene)
{

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		Vector3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder Vector3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			Vector2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
			vertex.TexCoords = Vector2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	

}

Texture StaticPlayer::loadMaterialTextures(TextureType typeName)
{
	string MapPath;
	switch (typeName) {
	case DiffuseMap:
		MapPath = texturePath + "Albedo.png";
		break;
	case MetallicMap:
		MapPath = texturePath + "Metallic.png";
		break;
	case NormalMap:
		MapPath = texturePath + "Normal.png";
		break;
	case RoughnessMap:
		MapPath = texturePath + "Roughness.png";
		break;
	default:
		break;
	}

	if (MapPath.empty()) {
		cout << "what map do you want me to find?" << endl;
	}
	Texture texture;


	//cout << "map path: " << MapPath << endl;
	texture.id = TextureFromFile(MapPath);
	texture.type = typeName;
	texture.path = MapPath;
	return texture;
}

GLuint StaticPlayer::TextureFromFile(string path)
{

	//std::string strFullPath;
	std::string sExecutableDirectory = Path_StripFilename(Path_GetExecutablePath());
	std::string strFullPath = Path_MakeAbsolute(string(path), sExecutableDirectory);
	cout << "map path: " << strFullPath << endl;
	//cout << filename << endl;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	std::vector<unsigned char> imageRGBA;
	unsigned nImageWidth, nImageHeight;

	//strFullPath = Path_MakeAbsolute(string(path), sExecutableDirectory);
	//std::cout << "cube map path: " << strFullPath << std::endl;
	unsigned nError = lodepng::decode(imageRGBA, nImageWidth, nImageHeight, strFullPath.c_str());

	if (nError != 0) { 
		cout << "error" << endl;
		return false; }



	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nImageWidth, nImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imageRGBA[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	imageRGBA.clear();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	

	//cout << filename << "textureID: " << textureID << endl;

	return textureID;
}

void StaticPlayer::setup()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a Vector3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//cout << "the ground vertices: " << endl;
	//for (auto i = 0; i < vertices.size(); i++)
	//	cout << vertices[i].Position.x << " " << vertices[i].Position.y << " " << vertices[i].Position.z << endl;


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}


#pragma once


class Texture
{
private:
	unsigned int _texID;
	unsigned char* _data;
	int _width, _height;
	//static Texture white;

public:
	Texture(void) : _width(0), _height(0) {}
	Texture(const char* path);
	~Texture(void);

	bool LoadFromFile(const char* path);

	void Bind(int slot) const;
	void Unbind(int slot) const;

	inline bool HasData() const { return _width > 0; }
	inline int GetWidth() const { return _width; }
	inline int GetHeight() const { return _height; }

	static Texture GetBlank() 
	{
		/*if (!white.HasData())
			white.LoadFromFile("res/white.png");
		return white;*/
		return Texture();
	}
};
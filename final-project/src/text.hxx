#include "cs237.hxx"

class Text
{
	cs237::texture2D *scoreImage;
	cs237::ShaderProgram    *_shader;
	cs237::vec2f screenSize;


	int texLoc;
	int screenLoc;
    GLuint vaoId;

	public:
	Text(cs237::vec2f screenSize);
	void Render(cs237::vec2f const &screenSize);
};
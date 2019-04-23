#ifndef RENDERSYSTEMINTERFACE_H_
#define RENDERSYSTEMINTERFACE_H_

#include <string>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

namespace Ogre {
	class OverlayManager;
	class Camera;
	class OverlayContainer;
	class Overlay;
	class OverlayElement;
	class TextAreaOverlayElement;
	class SceneManager;
	class Viewport;
	class SceneNode;
	class Entity;
	class Light;
	class ColourValue;

	template <int dims, typename T> class Vector;
	typedef float Real;
	typedef Vector<3, Real> Vector3;
	class Quaternion;
};

class nap_vector3;
class nap_quat;
using OgrePair = std::pair<Ogre::SceneNode*, Ogre::Entity*>;

class RenderSystemInterface
{
private:
	Ogre::OverlayManager *overlayManager = nullptr;	
	Ogre::SceneManager * mScnMgr = nullptr;
	Ogre::Camera* camera = nullptr;
	std::string panelName = "PanelName";
	std::string overlayName = "OverlayName";
	std::string currentRenderingScene;

	static RenderSystemInterface* instance_; //singleton pattern
	RenderSystemInterface();
	virtual ~RenderSystemInterface();

public:
	static RenderSystemInterface* getSingleton();
	static void shutdownSingleton();
	//Resto de interfaz shaders (?), animacion, camara, viewport...
	/// Defines the type of light
	enum LightTypes
	{
		/// Point light sources give off light equally in all directions, so require only position not direction
		LT_POINT = 0,
		/// Directional lights simulate parallel light beams from a distant source, hence have direction but no position
		LT_DIRECTIONAL = 1,
		/// Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff
		LT_SPOTLIGHT = 2
	};

	/*
	 *Devuelve el RootNode de la escena
	 */
	inline Ogre::SceneNode* getRootSceneNode();
	/*
	 *Devuelve el Scene Manager
	 */
	inline Ogre::SceneManager* getSceneManager();
	inline void setSceneManager(Ogre::SceneManager* s);
	/*
	 *Busca la entidad por nombre y la devuelve
	 */
	inline Ogre::Entity* getEntityByName(std::string name);
	/*
	 *Crea una entidad + nodo segun el nombre
	 */
	OgrePair createOgreEntity(std::string name, std::string meshName = "");
	/*
	 *Crea un nodo vacio
	 */
	Ogre::SceneNode* createEmpty(std::string name);
	/*
	 *Crea una luz del tipo elegido. La direccion se setea con el nodo (node->setDirection(Ogre::Vector3(x, y, z));)
	 */
	Ogre::SceneNode* createLight(std::string name, LightTypes type, Ogre::ColourValue diffColor, Ogre::ColourValue specColor, Ogre::Real range);
	/*
     *set lights range, maths inside
	*/
	void setLightRange(Ogre::Light *L, Ogre::Real Range);
	 /*Crea un nodo vacio
	 * UP NO PUEDE SER IGUAL A NORMAL
	 */
	OgrePair createPlane(std::string name, Ogre::Vector3 Normal, Ogre::Real w, Ogre::Real h, Ogre::Vector3 up);
	/*
	 *set de la luz ambiental
	 */
	void setAmbientLight(Ogre::ColourValue color);
	/*
	 *A�ade un hijo al nodo que le pases
	 */
	Ogre::SceneNode* addChild(Ogre::SceneNode* father, std::string name, std::string meshName);
	/*
	 *A�ade un hijo (ya creado) al nodo que le pases
	 */
	void addChild(Ogre::SceneNode* father, Ogre::SceneNode* child);
	/*
	 *Set del material a una entidad
	 */
	void setMaterial(std::string entity, std::string material);
	/*
	 *Set del material con acceso a la entidad
	 */
	void setMaterial(Ogre::Entity* entity, std::string material);
	/*
	 *Get node by nombre
	 */
	Ogre::SceneNode* getNode(std::string name);
	/*
	 *Get camera (modify nearClipDistance, FarClip, AspectRatio...)
	 */
	inline Ogre::Camera* getCamera();
	/*
	 *Get cameraNode (modify position, lookAt...)
	 */
	Ogre::SceneNode* getCameraNode();
	/*
	 *get the viewport
	*/
	inline Ogre::Viewport* getViewport();
	/*
	 *creates text element
	*/
	Ogre::TextAreaOverlayElement* createText(std::string nodeName, std::string text, int x = 0, int y = 0, std::string fontName = "HackReg");
	/*
	*SetText with std::string
	*/
	void setText(Ogre::TextAreaOverlayElement* element, std::string szString); // now You can use std::string as text
	/*
	 *Set text position on overlay
	*/
	void setTextPosition(Ogre::TextAreaOverlayElement* element, float x, float y);
	/*
	 *Set size of the entire text
	 */
	void setTextSize(Ogre::TextAreaOverlayElement* element, float size);
	/*
	 *Not working
	 */
	void setTextCenteredPosition(Ogre::TextAreaOverlayElement* element, float x, float y);
	/*
	*Set text color, rgb values between and alpha value [0, 1]
	*/
	void setTextColour(Ogre::TextAreaOverlayElement* element, float R, float G, float B, float I);
	/*
	*Set Top-font color, rgb values between and alpha value [0, 1]
	*/
	void setTextColourTop(Ogre::TextAreaOverlayElement* element, float R, float G, float B, float I);

	/*
	 *Set Dropshadow color, rgb values and alpha value between [0, 1]
	 */
	void setTextColourBot(Ogre::TextAreaOverlayElement* element, float R, float G, float B, float I);
	/*
	 *create overlay element
	 */
	Ogre::OverlayElement * createOverlayElement(std::string type, std::string name);	
	/*
	 *set overlay element dimensions (1.0f = screen width/height)
	 */
	void setOverlayElementDimensions(Ogre::OverlayElement* e, float w, float h);	
	/*
	 *set overlay element position (1.0f = screen width/height)
	 */
	void setOverlayElementPosition(Ogre::OverlayElement* e, float x, float y);
	/*
	 *set overlay element position (using its center and not its 0, 0 position)
	 */
	void setOverlayElementCenteredPosition(Ogre::OverlayElement* e, float x, float y);
	/*
	 *gets overlay element
	 */
	Ogre::OverlayElement * getOverlayElement(std::string name);
	/*
	 *set overlayMaterial
	 */
	void setOverlayElementMaterial(Ogre::OverlayElement* e, std::string matName);
	/*
	 * Returns the dest rotation
	 */	
	Ogre::Quaternion getRotationFrom_To(Ogre::Vector3 src, Ogre::Vector3 dest);
	/*
	* Returns the dest rotation, uses our nap_vector3 and nap_quat
	*/
	nap_quat getRotationFrom_To(nap_vector3 src, nap_vector3 dest);
	/*
	 *skybox
	 */
	void setSkyBox(std::string material, float distance = 5000);

	/*
	 *sets up all the neccesary elements. Starts rendering a new scene
	*/
	void setRenderingScene(std::string scene);

	std::string getCurrentRenderingScene();

	void addToPanel(std::string name, Ogre::OverlayElement* elemt);
};

#endif //RENDERSYSTEMINTERFACE_H_
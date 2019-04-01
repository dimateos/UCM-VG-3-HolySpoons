#ifndef RENDERSYSTEMINTERFACE_H_
#define RENDERSYSTEMINTERFACE_H_

#include <string>

namespace Ogre {
	class OverlayManager;
	class Camera;
	class OverlayContainer;
	class Overlay;
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

	static RenderSystemInterface* instance_; //singleton pattern
	RenderSystemInterface(Ogre::SceneManager * mScnMgr);
	virtual ~RenderSystemInterface();

public:
	static RenderSystemInterface* createSingleton(Ogre::SceneManager * mScnMgr);
	static RenderSystemInterface* getSingleton();
	void closeInterface();
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
	Ogre::SceneNode* createLight(std::string name, LightTypes type, Ogre::ColourValue color);
	/*
	 *Crea un nodo vacio
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

	inline Ogre::Viewport* getViewport();

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
	 *create overlayPanel, used for images
	 */
	Ogre::OverlayContainer * createOverlayContainer();

	/*
	 * Returns the dest rotation
	 */	
	Ogre::Quaternion getRotationFrom_To(Ogre::Vector3 src, Ogre::Vector3 dest);
	/*
	* Returns the dest rotation, uses our nap_vector3 and nap_quat
	*/
	nap_quat getRotationFrom_To(nap_vector3 src, nap_vector3 dest);
};

#endif //ENDERSYSTEMINTERFACE_H_
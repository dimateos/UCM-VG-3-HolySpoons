#include <iostream>
#include <OgreInterface.h>

int main()
{
	OgreBites::OgreInterface* renderInterface = new OgreBites::OgreInterface();
	renderInterface->initApp();
	system("pause");
	delete renderInterface;
	return 0;
}

#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__
#include <string>

class UIManager
{
public:
    UIManager();
    void initialize();
    void render();
    void display();

    void keyboardEvent(unsigned char key, int x, int y);
	void specialEvent(int key,int x,int y);
	void mouseEvent(int button, int state, int x, int y);
	void motionEvent(int x,int y);
	void reshapeEvent(int w,int h);


public:
    const std::string GetFilePath();
private:
    virtual ~UIManager();

    std::string m_FilePath;
    bool m_bOpenBVH;


};

#endif
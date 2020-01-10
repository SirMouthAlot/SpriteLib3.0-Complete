#ifndef __SELECTABLE_H__
#define __SELECTABLE_H__

#include <string>

class Selectable
{
public:
	Selectable(std::string name);
	~Selectable();

	//Getters
	bool* GetSelected() const;
	std::string GetName() const;

	//Setters
	void SetSelected(bool selected);
	void SetName(std::string name);
private:
	bool* m_selected = nullptr;
	std::string m_name = "";
};

#endif // !__SELECTABLE_H__

#include "Selectable.h"

Selectable::Selectable(std::string name)
{
	m_selected = new bool(true);
	m_name = name;
}

Selectable::~Selectable()
{
	if (m_selected != nullptr)
	{
		delete m_selected;
		m_selected = nullptr;
	}
}

bool* Selectable::GetSelected() const
{
	return m_selected;
}

std::string Selectable::GetName() const
{
	return m_name;
}

void Selectable::SetSelected(bool selected)
{
	*m_selected = selected;
}

void Selectable::SetName(std::string name)
{
	m_name = name;
}

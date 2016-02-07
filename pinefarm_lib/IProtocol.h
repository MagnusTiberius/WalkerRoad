#pragma once
class IProtocol
{
public:
	IProtocol();
	~IProtocol();

public:
	virtual void Connect() = 0;
};


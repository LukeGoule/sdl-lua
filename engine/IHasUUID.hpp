#pragma once
#ifndef IHASUUID_HPP
#define IHASUUID_HPP

class IHasUUID {
public:
	IHasUUID() {}
	virtual ~IHasUUID() {}


	virtual const char* getUUID() = 0;
};

#endif
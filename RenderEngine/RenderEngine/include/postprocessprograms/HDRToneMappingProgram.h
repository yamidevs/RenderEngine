#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class HDRToneMappingProgram : public PostProcessProgram
	{
	public:
		static const std::string PROGRAM_NAME;
	private:
		unsigned int uExposure;
		unsigned int uGamma;
	public:
		HDRToneMappingProgram(std::string name, unsigned long long params);
		HDRToneMappingProgram(const HDRToneMappingProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 & proj);
	};

	// ====================================================================

	class HDRToneMappingProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}
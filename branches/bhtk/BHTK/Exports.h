#pragma once

#if defined(EXPORTING)
#define EXPORT __declspec(dllexport)
#elif defined(EXPORTED)
#define EXPORT __declspec(dllimport)
#endif

#if defined(_DEFINE_VAR)
	#define VAR
#else
	#define VAR extern
#endif
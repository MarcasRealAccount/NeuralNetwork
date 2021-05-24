workspace("NeuralNetwork")
	platforms({ "x64" })
	configurations({ "Debug", "Release", "Dist" })
	
	cppdialect("C++17")
	cdialect("C11")
	exceptionhandling("On")
	flags({ "MultiProcessorCompile" })
	rtti("Off")
	objdir("bin-int/%{cfg.buildcfg}/%{prj.name}")
	targetdir("bin/%{cfg.buildcfg}/%{prj.name}")
	
	defines({ "_CRT_SECURE_NO_WARNINGS" })
	
	filter("configurations:Debug")
		defines({ "_DEBUG" })
		symbols("On")
		optimize("Off")
		
	filter("configurations:Release")
		defines({ "_RELEASE" })
		symbols("On")
		optimize("Full")
		
	filter("configurations:Dist")
		defines({ "_DIST" })
		symbols("Off")
		optimize("Full")
	
	filter("system:windows")
		toolset("msc")
		defines({ "NOMINMAX" })
	
	filter("system:not windows")
		toolset("gcc")
	
	filter("system:linux")
		debugenvs({ "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../%{OUTDIR}" })
	
	filter({})
	
	group("Libs")
project("Armadillo")
	kind("StaticLib")
	location("libs")
	warnings("Off")
	
	includedirs({ "libs/armadillo/include" })
	files({ "libs/armadillo/src/**", "libs/armadillo/include/**" })
	
	group("")
project("DigitRecognition")
	kind("ConsoleApp")
	location("DigitRecognition")
	debugdir("DigitRecognition/run/")
	os.mkdir("DigitRecognition/run/")
	warnings("Extra")
	
	includedirs({ "DigitRecognition/src" })
	sysincludedirs({ "libs/armadillo/include" })
	files({ "DigitRecognition/src/**" })
	
	links({ "Armadillo" })
	
workspace("NeuralNetwork")
	startproject("NeuralNetwork")
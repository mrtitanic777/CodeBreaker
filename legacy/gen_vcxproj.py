# gen_vcxproj.py - generate MinecraftIndev.vcxproj listing every output/*.cpp for a
# Release | Xbox 360 build (cl -> link -> imagexex -> .xex via the XDK MSBuild targets).
import os, sys

OUT = os.path.join(os.path.dirname(__file__), 'output')
cpps = sorted(f for f in os.listdir(OUT) if f.endswith('.cpp'))

REL_LIBS = ('xapilib.lib;d3d9.lib;d3dx9.lib;xgraphics.lib;xboxkrnl.lib;xnet.lib;'
            'xaudio2.lib;xact3.lib;x3daudio.lib;xmcore.lib;vcomp.lib;xbdm.lib')

def clcompile(fn):
    if fn == 'stdafx.cpp':
        return ('    <ClCompile Include="' + fn + '">\n'
                '      <PrecompiledHeader>Create</PrecompiledHeader>\n'
                '    </ClCompile>\n')
    return '    <ClCompile Include="' + fn + '" />\n'

items = ''.join(clcompile(f) for f in cpps)

proj = '''<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Release|Xbox 360">
      <Configuration>Release</Configuration>
      <Platform>Xbox 360</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <ProjectGuid>{A1B2C3D4-E5F6-47A8-9B0C-1D2E3F405161}</ProjectGuid>
    <Keyword>Xbox360Proj</Keyword>
    <RootNamespace>MinecraftIndev</RootNamespace>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Xbox 360'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <CharacterSet>MultiByte</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Xbox 360'">
    <LinkIncremental>false</LinkIncremental>
    <OutDir>$(MSBuildProjectDirectory)\\_xexbuild\\</OutDir>
    <IntDir>$(MSBuildProjectDirectory)\\_xexbuild\\obj\\</IntDir>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|Xbox 360'">
    <ClCompile>
      <WarningLevel>Level1</WarningLevel>
      <PrecompiledHeader>Use</PrecompiledHeader>
      <PrecompiledHeaderFile>stdafx.h</PrecompiledHeaderFile>
      <Optimization>Disabled</Optimization>
      <ExceptionHandling>false</ExceptionHandling>
      <RuntimeTypeInfo>true</RuntimeTypeInfo>
      <BufferSecurityCheck>false</BufferSecurityCheck>
      <RuntimeLibrary>MultiThreaded</RuntimeLibrary>
      <PreprocessorDefinitions>NDEBUG;_XBOX;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <AdditionalIncludeDirectories>$(MSBuildProjectDirectory);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <MultiProcessorCompilation>true</MultiProcessorCompilation>
      <MinimalRebuild>false</MinimalRebuild>
      <TreatWarningAsError>false</TreatWarningAsError>
    </ClCompile>
    <Link>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <AdditionalDependencies>@@LIBS@@</AdditionalDependencies>
      <SetChecksum>true</SetChecksum>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <ClInclude Include="stdafx.h" />
  </ItemGroup>
  <ItemGroup>
@@ITEMS@@  </ItemGroup>
  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />
</Project>
'''.replace('@@LIBS@@', REL_LIBS).replace('@@ITEMS@@', items)

dest = os.path.join(OUT, 'MinecraftIndev.vcxproj')
with open(dest, 'w') as f:
    f.write(proj)
print('Wrote %s with %d .cpp files' % (dest, len(cpps)))

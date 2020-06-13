// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// ***********************************************************************
#pragma once

#include "StdBhvTree.h"
#include <vector>
#include "tsl\ordered_map.h"
#include "../BasicXMLGUI/UIntVector.h"
#include <map>
//Loading from Niflib
#include "niflib.h"
#include "obj/NiAVObjectRef.h"
#include "obj/NiAVObject.h"

/// <summary>
/// Class named NifNode(derive into NifNode subclass into view). (Node links to other nodes in node bank)
/// Held within NodeDictionary with int key
/// </summary>
class NifNode
{public:
	/// <summary>
	/// Index position of ParentNode (EmptyNode=inside Root Level by default)
	/// </summary>
	int ParentIndex;
	
	/// <summary>
	/// The indexes of all child nodes
	/// </summary>
	UIntVector ChildNodes;

	/// <summary>
	/// If true, the Node is Open(Temporarily auto-set as OPEN)
	/// </summary>
	const static bool bOpen = true;
	
	/// <summary>
	/// Node Coordinate Data(Current Position of Node in Tree)
	/// </summary>
	CRect		CoordData;
	
	Niflib::NiObject ObjectData;
	
	std::string GetNodeType()
	{
	
	}
	
	NifNode()
	{
		CoordData.SetRectEmpty();
	}
	
	/// <summary>
	/// Finalizes an instance of the <see cref="NifNode"/> class.
	/// </summary>
	~NifNode(){}
};

class NifNodeTree : public std::map<unsigned int, NifNode>
{
//Keys to the dictionary are equivalent of old internal block number code
	private:
		using NiObjectList = std::vector<Niflib::Ref<Niflib::NiObject> >;
public:
	//UIntVector RootNodes;
	unsigned int RootIndex;
	Niflib::NiObjectRef FindRoot_NodeTree(vector<Niflib::NiObjectRef> const & objects)
	{
		using namespace Niflib;
		//--Look for a NiNode that has no parents--//
		//Find the first NiObjectNET derived object
		NiAVObjectRef root;
		for(size_t i = 0; i < objects.size(); ++i)
		{
			root = DynamicCast<NiAVObject>(objects[i]);
			if(root != NULL)
			{
				break;
			}
		}
		//Make sure a node was found, if not return first node
		if(root == NULL)
			return objects[0];
		//Move up the chain to the root node
		while(root->GetParent() != NULL)
		{
			root = StaticCast<NiAVObject>(root->GetParent());
		}
		return StaticCast<NiObject>(root);
	}

	/// <summary>
	/// Alternative version of loading code based on niflib intro_page.html documentation
	/// </summary>
	void BasicNifTreeLoad(std::string current_file)
	{
        try 
		{
            //Niflib Function Call
            vector<NiObject> objects = ReadNifList(current_file);
        }
        catch (exception & e) {
            cout << "Error: " << e.what() << endl;
            return 0;
        }
        catch (...) {
            cout << "Unknown Exception." << endl;
            return 0;
        }
	}
	void NifNodeTree::ReadNifNodeTree(std::istream & in, NiObjectList & missing_link_stack, Niflib::NifInfo * info = NULL)
	{
		using namespace Niflib;
		if(Size() != 0)
		{
			ResetData();
		}

		//--Read Header--//
		Header header;
		hdrInfo hinfo(&header);

		// set the header pointer in the stream
		in >> hinfo;

		//Create a new NifInfo if one isn't given.
		bool delete_info = false;
		if(info == NULL)
		{
			info = new NifInfo();
			delete_info = true;
		}

		//Read header.
		*info = header.Read(in);

		//If NifInfo structure is provided, fill it with info from header
		info->version = header.version;
		info->userVersion = header.userVersion;
		info->userVersion2 = header.userVersion2;
		info->endian = EndianType(header.endianType);
		info->creator = header.exportInfo.creator.str;
		info->exportInfo1 = header.exportInfo.exportInfo1.str;
		info->exportInfo2 = header.exportInfo.exportInfo2.str;

	#ifdef DEBUG_HEADER_FOOTER
		//Print debug output for header
		std::cout << header.asString();
	#endif

	#ifdef PRINT_OBJECT_NAMES
		std::cout << endl << "Reading Objects:";
	#endif

		//--Read Objects--//
		size_t numObjects = header.numBlocks;
		map<unsigned, NiObjectRef> objects; //Map to hold objects by number
		//vector<NiObjectRef> obj_list; //Vector to hold links in the order they were created.
		list<unsigned int> link_stack; //List to add link values to as they're read in from the file
		std::string objectType;
		stringstream errStream;

		std::streampos headerpos = in.tellg();
		std::streampos nextobjpos = headerpos;

		//Loop through all objects in the file
		size_t i = 0;
		while(true)
		{
			// Check if the size information matches in version 20.3 and greater
			if(header.version >= VER_20_3_0_3)
			{
				if(nextobjpos != in.tellg())
				{
					// incorrect positioning seek to expected location
					in.seekg(nextobjpos);
				}
				// update next location
				nextobjpos += header.blockSize[i];
			}

			//Check for EOF
			if(in.eof())
			{
				errStream << "End of file reached prematurely.  This NIF may be corrupt or improperly supported." << endl;
				throw runtime_error(errStream.str());
			}

			// Starting position of block in stream
			std::streampos startobjpos = in.tellg();

			//There are two main ways to read objects
			//One before version 5.0.0.1 and one after
			if(header.version >= 0x05000001)
			{
				//From version 5.0.0.1 to version 10.0.1.106  there is a zero byte at the begining of each object

				if(header.version <= VER_10_1_0_106)
				{
					unsigned int checkValue = ReadUInt(in);
					if(checkValue != 0)
					{
						//Throw an exception if it's not zero
						errStream << "Read failue - Bad object position.  Invalid check value:  " << checkValue << endl;
						throw runtime_error(errStream.str());
					}
				}

				// Find which NIF object type this is by using the header arrays
				objectType = header.blockTypes[header.blockTypeIndex[i]];

	#ifdef PRINT_OBJECT_NAMES
				std::cout << endl << i << ":  " << objectType;
	#endif
			}
			else
			{
				// Find which object type this is by reading the string at this location
				unsigned int objectTypeLength = ReadUInt(in);
				if(objectTypeLength > 30 || objectTypeLength < 6)
				{
					errStream << "Read failue - Bad object position.  Invalid Type Name Length:  " << objectTypeLength << endl;
					throw std::runtime_error(errStream.str());
				}
				char* charobjectType = new char[objectTypeLength + 1];
				in.read(charobjectType, objectTypeLength);
				charobjectType[objectTypeLength] = 0;
				objectType = string(charobjectType);
				delete [] charobjectType;

	#ifdef PRINT_OBJECT_NAMES
				std::cout << endl << i << ":  " << objectType;
	#endif

				if(header.version < VER_3_3_0_13)
				{
					//There can be special commands instead of object names
					//in these versions

					if(objectType == "Top Level Object")
					{
						//Just continue on to the next object
						continue;
					}

					if(objectType == "End Of File")
					{
						//File is finished
						break;
					}
				}
			}

		//Need to create alternative version of NiObjectRef that skips using Niflib::ObjectRegistry altogether later
		if(CreateObject(objectType)==false)
		{
			errStream << "Unknown object type encountered during file read:  " << objectType << endl;
			throw runtime_error(errStream.str());
		}
		size_t index;
		if(header.version < VER_3_3_0_13)
		{
			//These old versions have a pointer value after the name
			//which is used as the index
			index = ReadUInt(in);
		}
		else
		{
			//These newer verisons use their position in the file as their index
			index = i;
		}
		////Read new object data from Stream
		//FixCurrentNodeData();
		//insert_or_assign(index, Value);//index = internal_block_number
		ReadObjectFromStream(objectType, in, link_stack, *info);

			// Ending position of block in stream
			std::streampos endobjpos = in.tellg();

			// Check if the size information matches
			if(header.version >= VER_20_3_0_3)
			{
				std::streamsize calcobjsize = endobjpos - startobjpos;
				unsigned int objsize = header.blockSize[i];
				if(calcobjsize != objsize)
				{
					errStream << "Object size mismatch occurred during file read:" << endl;
					errStream << "====[ " << "Object " << i << " | " << objectType << " ]====" << endl;
					errStream << "  Start: " << startobjpos << "  Expected Size: " << objsize << "  Read Size: " << calcobjsize << endl;
					errStream << endl;
				}
			}

	#ifdef PRINT_OBJECT_CONTENTS
			std::cout << endl << new_obj->asString() << endl;
	#endif

			if(header.version >= VER_3_3_0_13)
			{
				//We know the number of objects, so increment the count
				//and break if we've finished
				++i;
				if(i >= numObjects)
				{
					break;
				}
			}
		}

		//--Read Footer--//
		Footer footer;
		footer.Read(in, link_stack, *info);

	#ifdef DEBUG_HEADER_FOOTER
		//Print footer debug output
		footer.asString();
	#endif

		// Check for accumulated warnings
		if(errStream.tellp() > 0)
		{
			throw runtime_error(errStream.str());
		}

		//delete info if it was dynamically allocated
		if(delete_info)
		{
			delete info;
		}

		// clear the header pointer in the stream.  Should be in try/catch block
		hdrInfo hinfo2(NULL);
		in >> hinfo2;
		
		//Now Arrange and add nodes from obj_list to NodeTree in correct order(move from LooseNodes section to tree/Extra Data links as find nodes)
		bool RootNotFound=true;
		NifNode* TargetNode = nullptr;
		for (NifNodeTree::iterator targetNodeIndex = this->begin(), EndIndex = this->end(); TargetInfoNode==nullptr&&RootNotFound; ++targetNodeIndex)
        {
			if(targetNodeIndex*.GetNodeType()=="NiAVObjectRef"&&targetNodeIndex*.ObjectData->GetParent()==NULL)//If has no parent assume is Root
			{
				RootNotFound = false;
				RootIndex = targetNodeIndex->first;
			}
		}
	}

	NifNodeTree(string const & FileName)
	{
	
	}
	NifNodeTree()
	{}
	~NifNodeTree()
	{}
};
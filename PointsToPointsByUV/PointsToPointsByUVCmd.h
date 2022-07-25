#ifndef _PointsToPointsByUVCmd
#define _PointsToPointsByUVCmd

#define uvshortFlag             "-uv"
#define uvlongFlag             "-uvActive"


#include <maya/MPxCommand.h>
#include <maya/MSelectionList.h>
#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MIntArray.h>
#include <maya/MArgList.h>

class PointsToPointsByUV : public MPxCommand
{

public:
				PointsToPointsByUV();
	virtual		~PointsToPointsByUV();

	MStatus		doIt( const MArgList& arglist );
	MStatus		redoIt();
	MStatus		undoIt();
	bool		isUndoable() const;

	static		void* creator();

private:
	// Store the data you will need to undo the command here
	MDagPath dagpath1, dagpath2;
	MPointArray pointsArray1, pointsArray2;

	int uvIndex;

public:
	bool isUVactive = true;
	bool isHasArguments = true;
};

#endif

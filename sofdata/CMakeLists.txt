# Create a library called "libR3BSofData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES} )

set(INCLUDE_DIRECTORIES
${R3BROOT_SOURCE_DIR}/r3bdata
${R3BROOT_SOURCE_DIR}/r3bdata/califaData
${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
${R3BSOF_SOURCE_DIR}/sofdata
${R3BSOF_SOURCE_DIR}/sofdata/atData
${R3BSOF_SOURCE_DIR}/sofdata/sciData
${R3BSOF_SOURCE_DIR}/sofdata/trimData
${R3BSOF_SOURCE_DIR}/sofdata/twimData
${R3BSOF_SOURCE_DIR}/sofdata/mwpcData
${R3BSOF_SOURCE_DIR}/sofdata/tofwData
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR} )

link_directories( ${LINK_DIRECTORIES})

set(SRCS
atData/R3BSofATPoint.cxx
atData/R3BSofAtMappedData.cxx
mwpcData/R3BSofMWPCPoint.cxx
mwpcData/R3BSofMwpcMappedData.cxx
mwpcData/R3BSofMwpcCalData.cxx
mwpcData/R3BSofMwpcHitData.cxx
twimData/R3BSofTWIMPoint.cxx
tofwData/R3BSofToFWPoint.cxx
sciData/R3BSofSciMappedData.cxx
sciData/R3BSofSciTcalData.cxx
sciData/R3BSofSciSingleTcalData.cxx
sciData/R3BSofSciCalData.cxx
tofwData/R3BSofToFWMappedData.cxx
tofwData/R3BSofToFWTcalData.cxx
tofwData/R3BSofToFWCalData.cxx
tofwData/R3BSofTofWHitData.cxx
twimData/R3BSofTwimMappedData.cxx
twimData/R3BSofTwimCalData.cxx
twimData/R3BSofTwimHitData.cxx
trimData/R3BSofTrimMappedData.cxx
)


# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
SET(HEADERS ${HEADERS} ${R3BROOT_SOURCE_DIR}/r3bdata/R3BDetectorList.h )

set(LINKDEF  R3BSofDataLinkDef.h)
set(LIBRARY_NAME R3BSofData)
set(DEPENDENCIES
    Base FairTools R3Bbase R3BData Core m)

GENERATE_LIBRARY()

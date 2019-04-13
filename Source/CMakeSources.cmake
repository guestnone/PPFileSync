set(PPFS_INC_NOFILTER
	"PPFSPrerequisites.h"
	"PPFSScanner.h"
	"PPFSSynchronizer.h"
	"PPFSBase.c"
)

set(PPFS_SRC_NOFILTER
	"PPFSMain.c"
	"PPFSScanner.c"
	"PPFSSynchronizer.c"
	"PPFSBase.c"
)

source_group("" FILES ${PPFS_INC_NOFILTER} ${PPFS_SRC_NOFILTER})

set(PPFS_SRC
	${PPFS_INC_NOFILTER}
	${PPFS_SRC_NOFILTER}
)
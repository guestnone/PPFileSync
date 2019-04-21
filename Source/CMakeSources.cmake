set(PPFS_INC_NOFILTER
	"PPFSPrerequisites.h"
	"PPFSScanner.h"
	"PPFSSynchronizer.h"
	"PPFSBase.c"
)

set(PPFS_SRC_NOFILTER
	"PPFSScanner.c"
	"PPFSSynchronizer.c"
	"PPFSBase.c"
)
set(PPFS_SRC_MAIN_NOFILTER
	"PPFSMain.c"
)

source_group("" FILES ${PPFS_INC_NOFILTER} ${PPFS_SRC_NOFILTER} ${PPFS_SRC_MAIN_NOFILTER})

set(PPFS_SRC
	${PPFS_INC_NOFILTER}
	${PPFS_SRC_NOFILTER}
	${PPFS_SRC_MAIN_NOFILTER}
)
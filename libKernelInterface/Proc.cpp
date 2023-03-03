#include "Common.h"
#include "Proc.h"
#include "Misc.h"

namespace Kernel
{
	struct iovec {
		void* iov_base;  // Pointer to the data
		size_t iov_len;   // Length of the data
	};

	enum	uio_rw { UIO_READ, UIO_WRITE };

	/* Segment flag values. */
	enum uio_seg {
		UIO_USERSPACE,		/* from user data space */
		UIO_SYSSPACE,		/* from system space */
		UIO_NOCOPY			/* don't copy, already in object */
	};

	struct uio {
		struct	iovec* uio_iov;		/* scatter/gather list */
		int	uio_iovcnt;				/* length of scatter/gather list */
		off_t	uio_offset;			/* offset in target object */
		ssize_t	uio_resid;			/* remaining bytes to process */
		enum	uio_seg uio_segflg;	/* address space */
		enum	uio_rw uio_rw;		/* operation */
		Thread* uio_td;				/* owner */
	};

	Proc* GetProcByPid(int pid)
	{
		auto kernelBase = GetKernelBase();
		Proc* currentProc = *(Proc**)(kernelBase + OffsetTable->allproc);

		while (currentProc != nullptr)
		{
			if (currentProc->pid == pid)
			{
				return currentProc;
			}

			currentProc = currentProc->ProcList.next;
		}

		return nullptr;
	}

	bool ReadWriteProcessMemory(Proc* proc, void* addr, void* data, uint32_t len, bool write)
	{
		if (proc == nullptr)
		{
			printf("ReadWriteProcessMemory(): Invalid Process.\n");
			return false;
		}

		if (addr == nullptr)
		{
			printf("ReadWriteProcessMemory(): Invalid address.\n");
			return false;
		}

		if (data == nullptr || len <= 0)
		{
			printf("ReadWriteProcessMemory(): Invalid data.\n");
			return false;
		}

		iovec IoVec;
		memset(&IoVec, 0, sizeof(IoVec));
		IoVec.iov_base = data;
		IoVec.iov_len = len;

		uio Uio;
		memset(&Uio, 0, sizeof(Uio));
		Uio.uio_iov = &IoVec;
		Uio.uio_iovcnt = 1;
		Uio.uio_offset = (uint64_t)addr;
		Uio.uio_resid = (uint64_t)len;
		Uio.uio_segflg = UIO_SYSSPACE;
		Uio.uio_rw = write ? UIO_WRITE : UIO_READ;
		Uio.uio_td = proc->ThreadList.first; 

		auto res = ((int(*)(...))(GetKernelBase() + OffsetTable->proc_rwmem))(proc, &Uio);

		if (res != 0)
		{
			printf("ReadWriteProcessMemory(): proc_rwmem failed with the error %llX\n", res);
			return false;
		}

		return true;
	}
}

 * Copyright (c) 2000-2019 Apple Inc. All rights reserved.
 *
 *
 *
 *
#include <machine/machine_routines.h>
#include <kern/kalloc.h>        /* kalloc()/kfree() */
#include <kern/clock.h>         /* delay_for_interval() */
#include <libkern/OSAtomic.h>   /* OSAddAtomic() */
#if !CONFIG_EMBEDDED
#include <console/video_console.h>
#endif
#include <vm/vm_protos.h>       /* vnode_pager_vrele() */
#include <vfs/vfs_disk_conditioner.h>
#include <libkern/section_keywords.h>

int     vttoif_tab[9] = {
extern void             memory_object_mark_io_tracking(
	memory_object_control_t         control);
extern int paniclog_append_noflush(const char *format, ...);

	void * array,
	size_t nmembers,
	size_t member_size,
	int (*)(const void *, const void *));
__private_extern__ int unlink1(vfs_context_t, vnode_t, user_addr_t,
    enum uio_seg, int);
static void vnode_dropiocount(vnode_t);
static int unmount_callback(mount_t, __unused void *);

    struct vnode_attr *vap, uint32_t flags, int fmode, uint32_t *statusp, vfs_context_t ctx);
errno_t rmdir_remove_orphaned_appleDouble(vnode_t, vfs_context_t, int *);
#if CONFIG_JETSAM && (DEVELOPMENT || DEBUG)
extern int bootarg_no_vnode_jetsam;    /* from bsd_init.c default value is 0 */
#endif /* CONFIG_JETSAM && (DEVELOPMENT || DEBUG) */

boolean_t root_is_CF_drive = FALSE;

TAILQ_HEAD(freelst, vnode) vnode_free_list;     /* vnode free list */
TAILQ_HEAD(deadlst, vnode) vnode_dead_list;     /* vnode dead list */
TAILQ_HEAD(ragelst, vnode) vnode_rage_list;     /* vnode rapid age list */
int     rage_limit = 0;
int     ragevnodes = 0;

#define RAGE_LIMIT_MIN  100
#define RAGE_TIME_LIMIT 5

/*
 * ROSV definitions
 * NOTE: These are shadowed from PlatformSupport definitions, but XNU
 * builds standalone.
 */
#define PLATFORM_DATA_VOLUME_MOUNT_POINT "/System/Volumes/Data"
#define PLATFORM_VM_VOLUME_MOUNT_POINT "/private/var/vm"
struct mntlist mountlist;                       /* mounted filesystem list */
static int print_busy_vnodes = 0;                               /* print out busy vnodes */

#define VLISTCHECK(fun, vp, list)       \
	        panic("%s: %s vnode not on %slist", (fun), (list), (list));
#define VLISTNONE(vp)   \
	do {    \
	        (vp)->v_freelist.tqe_next = (struct vnode *)0;  \
	        (vp)->v_freelist.tqe_prev = (struct vnode **)0xdeadb;   \
#define VONLIST(vp)     \
#define VREMFREE(fun, vp)       \
	do {    \
	        VLISTCHECK((fun), (vp), "free");        \
	        TAILQ_REMOVE(&vnode_free_list, (vp), v_freelist);       \
	        VLISTNONE((vp));        \
	        freevnodes--;   \
#define VREMDEAD(fun, vp)       \
	do {    \
	        VLISTCHECK((fun), (vp), "dead");        \
	        TAILQ_REMOVE(&vnode_dead_list, (vp), v_freelist);       \
	        VLISTNONE((vp));        \
	        vp->v_listflag &= ~VLIST_DEAD;  \
	        deadvnodes--;   \
#define VREMASYNC_WORK(fun, vp) \
	do {    \
	        VLISTCHECK((fun), (vp), "async_work");  \
	        TAILQ_REMOVE(&vnode_async_work_list, (vp), v_freelist); \
	        VLISTNONE((vp));        \
	        vp->v_listflag &= ~VLIST_ASYNC_WORK;    \
	        async_work_vnodes--;    \
#define VREMRAGE(fun, vp)       \
	do {    \
	        if ( !(vp->v_listflag & VLIST_RAGE))                    \
	                panic("VREMRAGE: vp not on rage list");         \
	        VLISTCHECK((fun), (vp), "rage");                        \
	        TAILQ_REMOVE(&vnode_rage_list, (vp), v_freelist);       \
	        VLISTNONE((vp));                \
	        vp->v_listflag &= ~VLIST_RAGE;  \
	        ragevnodes--;                   \
	thread_t        thread = THREAD_NULL;
	if (rage_limit < RAGE_LIMIT_MIN) {
		rage_limit = RAGE_LIMIT_MIN;
	}
vnode_waitforwrites(vnode_t vp, int output_target, int slpflag, int slptimeout, const char *msg)
{
	int error = 0;
		slpflag |= PDROP;
		vnode_lock_spin(vp);
			if (output_target) {
				vp->v_flag |= VTHROTTLED;
			} else {
				vp->v_flag |= VBWAIT;
			}
			ts.tv_sec = (slptimeout / 100);
			ts.tv_nsec = (slptimeout % 1000)  * 10 * NSEC_PER_USEC * 1000;
vnode_startwrite(vnode_t vp)
{
	OSAddAtomic(1, &vp->v_numoutput);
		OSAddAtomic(-1, &vp->v_numoutput);
		if (vp->v_numoutput < 0) {
		}

		if (need_wakeup) {
		}
	struct cl_writebehind *wbp;
	 * state can change right after that. If their
	 */
	if (vp->v_dirtyblkhd.lh_first) {
		return 1;
	}

	if (!UBCINFOEXISTS(vp)) {
		return 0;
	}
	if (wbp && (wbp->cl_number || wbp->cl_scmap)) {
		return 1;
	}
	return 0;
	 * state can change right after that. If their
	 */
	if (vp->v_cleanblkhd.lh_first) {
		return 1;
	}
	return 0;
	int ret = 0;
		if (vp->v_type == VDIR) {
		}
		if (vp == skipvp) {
		}
		if ((flags & SKIPSYSTEM) && ((vp->v_flag & VSYSTEM) || (vp->v_flag & VNOFLUSH))) {
		}
		if ((flags & SKIPSWAP) && (vp->v_flag & VSWAP)) {
		}
		if ((flags & WRITECLOSE) && (vp->v_writecount == 0 || vp->v_type != VREG)) {
		}

		if ((vp->v_usecount != 0) && ((vp->v_usecount - vp->v_kusecount) != 0)) {
			ret = 1;
			if (print_busy_vnodes && ((flags & FORCECLOSE) == 0)) {
				vprint("vnode_umount_preflight - busy vnode", vp);
			} else {
				return ret;
			}
		} else if (vp->v_iocount > 0) {
			/* Busy if iocount is > 0 for more than 3 seconds */
			tsleep(&vp->v_iocount, PVFS, "vnode_drain_network", 3 * hz);
			if (vp->v_iocount > 0) {
				ret = 1;
				if (print_busy_vnodes && ((flags & FORCECLOSE) == 0)) {
					vprint("vnode_umount_preflight - busy vnode", vp);
				} else {
					return ret;
				}
			}
			continue;
	}

	return ret;
/*
		return 0;
	}
	if (mp->mnt_newvnodes.tqh_first != NULL) {
	}
	return 1;
int

		if (mvp) {
		} else {
		}

		if (mp->mnt_vnodelist.tqh_first) {
		} else {
		}
	return moved;
#if !CONFIG_EMBEDDED

#include <i386/panic_hooks.h>

struct vnode_iterate_panic_hook {
	panic_hook_t hook;
	mount_t mp;
	struct vnode *vp;
};

static void
vnode_iterate_panic_hook(panic_hook_t *hook_)
{
	struct vnode_iterate_panic_hook *hook = (struct vnode_iterate_panic_hook *)hook_;
	panic_phys_range_t range;
	uint64_t phys;

	if (panic_phys_range_before(hook->mp, &phys, &range)) {
		paniclog_append_noflush("mp = %p, phys = %p, prev (%p: %p-%p)\n",
		    hook->mp, phys, range.type, range.phys_start,
		    range.phys_start + range.len);
	} else {
		paniclog_append_noflush("mp = %p, phys = %p, prev (!)\n", hook->mp, phys);
	}

	if (panic_phys_range_before(hook->vp, &phys, &range)) {
		paniclog_append_noflush("vp = %p, phys = %p, prev (%p: %p-%p)\n",
		    hook->vp, phys, range.type, range.phys_start,
		    range.phys_start + range.len);
	} else {
		paniclog_append_noflush("vp = %p, phys = %p, prev (!)\n", hook->vp, phys);
	}
	panic_dump_mem((void *)(((vm_offset_t)hook->mp - 4096) & ~4095), 12288);
}
#endif //CONFIG_EMBEDDED
    void *arg)
	/*
	 * The mount iterate mutex is held for the duration of the iteration.
	 * This can be done by a state flag on the mount structure but we can
	 * run into priority inversion issues sometimes.
	 * Using a mutex allows us to benefit from the priority donation
	 * mechanisms in the kernel for locks. This mutex should never be
	 * acquired in spin mode and it should be acquired before attempting to
	 * acquire the mount lock.
	 */
	mount_iterate_lock(mp);

	/* If it returns 0 then there is nothing to do */
	if (retval == 0) {
		mount_iterate_unlock(mp);
		return ret;

#if !CONFIG_EMBEDDED
	struct vnode_iterate_panic_hook hook;
	hook.mp = mp;
	hook.vp = NULL;
	panic_hook(&hook.hook, vnode_iterate_panic_hook);
#endif
#if !CONFIG_EMBEDDED
		hook.vp = vp;
#endif
		if (vget_internal(vp, vid, (flags | VNODE_NODEAD | VNODE_WITHID | VNODE_NOSUSPEND))) {
			continue;
			/*
			if (vnode_reload(vp)) {
				/* vnode will be recycled on the refcount drop */
				vnode_put(vp);
				continue;
		case VNODE_RETURNED:
		case VNODE_RETURNED_DONE:
			vnode_put(vp);
			if (retval == VNODE_RETURNED_DONE) {
			}
			break;
		case VNODE_CLAIMED_DONE:
			mount_lock(mp);
			ret = 0;
			goto out;
		case VNODE_CLAIMED:
		default:
			break;
#if !CONFIG_EMBEDDED
	panic_unhook(&hook.hook);
#endif
	mount_iterate_unlock(mp);
	return ret;
void
mount_iterate_lock(mount_t mp)
{
	lck_mtx_lock(&mp->mnt_iter_lock);
}

void
mount_iterate_unlock(mount_t mp)
{
	lck_mtx_unlock(&mp->mnt_iter_lock);
}

	if (!locked) {
		mount_lock_spin(mp);
	}

	if (!locked) {
		mount_unlock(mp);
	}
	if (!locked) {
		mount_lock_spin(mp);
	}

	if (mp->mnt_count == 0 && (mp->mnt_lflag & MNT_LDRAIN)) {
		wakeup(&mp->mnt_lflag);
	}
	if (!locked) {
		mount_unlock(mp);
	}
	if (!locked) {
	}
	if (!locked) {
	}
	return retval;
	if (!locked) {
	}
	if (mp->mnt_iterref < 0) {
	} else {
		retval = 0;
	}
	if (!locked) {
	}
	return retval;
	while (mp->mnt_iterref) {
	}
	if (mp->mnt_iterref == -1) {
	}
int
	if (mp->mnt_lflag & MNT_LDRAIN) {
	}
	while (mp->mnt_count) {
	}
	if (mp->mnt_vnodelist.tqh_first != NULL) {
		panic("mount_refdrain: dangling vnode");
	}
	return 0;
void
mount_set_noreaddirext(mount_t mp)
{
	mount_lock(mp);
	mount_unlock(mp);
	if (mp->mnt_lflag & MNT_LDEAD) {
		return ENOENT;
	}
	mount_lock(mp);
	if (mp->mnt_lflag & MNT_LUNMOUNT) {
		if (flags & LK_NOWAIT || mp->mnt_lflag & MNT_LDEAD) {
			mount_unlock(mp);
			return ENOENT;

		/*
		 * Since all busy locks are shared except the exclusive
		 * lock granted when unmounting, the only place that a
		 * wakeup needs to be done is at the release of the
		 * exclusive lock at the end of dounmount.
		 */
		mp->mnt_lflag |= MNT_LWAIT;
		msleep((caddr_t)mp, &mp->mnt_mlock, (PVFS | PDROP), "vfsbusy", NULL);
		return ENOENT;
	mount_unlock(mp);

	/*
	 * Until we are granted the rwlock, it's possible for the mount point to
	 * change state, so re-evaluate before granting the vfs_busy.
	return 0;
vfs_rootmountfailed(mount_t mp)
{
	mount_t mp;
	strlcpy(mp->mnt_vfsstat.f_fstypename, vfsp->vfc_name, MFSTYPENAMELEN);
	return mp;
	struct vfstable *vfsp;
	for (vfsp = vfsconf; vfsp; vfsp = vfsp->vfc_next) {
		if (!strncmp(vfsp->vfc_name, fstypename,
		    sizeof(vfsp->vfc_name))) {
			break;
		}
	}
	if (vfsp == NULL) {
		return ENODEV;
	}
	if (*mpp) {
		return 0;
	}
	return ENOMEM;
#define DBG_MOUNTROOT (FSDBG_CODE(DBG_MOUNT, 0))
	struct vfs_attr vfsattr;
	int     error;
	vnode_t bdevvp_rootvp;
	KDBG_RELEASE(DBG_MOUNTROOT | DBG_FUNC_START);

		KDBG_RELEASE(DBG_MOUNTROOT | DBG_FUNC_END, error, 0);
		return error;

		KDBG_RELEASE(DBG_MOUNTROOT | DBG_FUNC_END, error, 1);
		return error;
	 * 4951998 - code we call in vfc_mountroot may replace rootvp
		if (vfsp->vfc_mountroot == NULL
		    && !ISSET(vfsp->vfc_vfsflags, VFC_VFSCANMOUNTROOT)) {
		}
		if (vfsp->vfc_mountroot) {
			error = (*vfsp->vfc_mountroot)(mp, rootvp, ctx);
		} else {
			error = VFS_MOUNT(mp, rootvp, 0, ctx);
		}

		if (!error) {
			if (bdevvp_rootvp != rootvp) {
				vnode_rele(bdevvp_rootvp);
				vnode_put(bdevvp_rootvp);
			if (mp->mnt_ioflags & MNT_IOFLAGS_FUSION_DRIVE) {
				root_is_CF_drive = TRUE;
			}

#if !CONFIG_EMBEDDED
			uint32_t speed;

			if (MNTK_VIRTUALDEV & mp->mnt_kern_flag) {
				speed = 128;
			} else if (disk_conditioner_mount_is_ssd(mp)) {
				speed = 7 * 256;
			} else {
				speed = 256;
			}
			vc_progress_setdiskspeed(speed);
#endif
			if (vfs_getattr(mp, &vfsattr, ctx) == 0 &&

				if ((vfsattr.f_capabilities.capabilities[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_DIR_HARDLINKS) &&
				    (vfsattr.f_capabilities.valid[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_DIR_HARDLINKS)) {
					mp->mnt_kern_flag |= MNTK_DIR_HARDLINKS;
				}
			if ((vfs_flags(mp) & MNT_MULTILABEL) == 0) {
				KDBG_RELEASE(DBG_MOUNTROOT | DBG_FUNC_END, 0, 2);
				return 0;
			}
			KDBG_RELEASE(DBG_MOUNTROOT | DBG_FUNC_END, 0, 3);
			return 0;

		if (error != EINVAL) {
		}
	KDBG_RELEASE(DBG_MOUNTROOT | DBG_FUNC_END, error ? error : ENODEV, 4);
	return ENODEV;
 * Mount the data volume of an ROSV volume group
int
vfs_mount_rosv_data(void)
#if CONFIG_ROSV_STARTUP
	int error = 0;
	int do_rosv_mounts = 0;
	error = vnode_get(rootvnode);
	if (error) {
		/* root must be mounted first */
		printf("vnode_get(rootvnode) failed with error %d\n", error);
		return error;
	}
	printf("NOTE: Attempting ROSV mount\n");
	struct vfs_attr vfsattr;
	VFSATTR_INIT(&vfsattr);
	VFSATTR_WANTED(&vfsattr, f_capabilities);
	if (vfs_getattr(rootvnode->v_mount, &vfsattr, vfs_context_kernel()) == 0 &&
	    VFSATTR_IS_SUPPORTED(&vfsattr, f_capabilities)) {
		if ((vfsattr.f_capabilities.capabilities[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_VOL_GROUPS) &&
		    (vfsattr.f_capabilities.valid[VOL_CAPABILITIES_FORMAT] & VOL_CAP_FMT_VOL_GROUPS)) {
			printf("NOTE: DETECTED ROSV CONFIG\n");
			do_rosv_mounts = 1;
	if (!do_rosv_mounts) {
		vnode_put(rootvnode);
		//bail out if config not supported
		return 0;
	}

	char datapath[] = PLATFORM_DATA_VOLUME_MOUNT_POINT; /* !const because of internal casting */

	/* Mount the data volume */
	printf("attempting kernel mount for data volume... \n");
	error = kernel_mount(rootvnode->v_mount->mnt_vfsstat.f_fstypename, NULLVP, NULLVP,
	    datapath, (rootvnode->v_mount), 0, 0, (KERNEL_MOUNT_DATAVOL), vfs_context_kernel());

	if (error) {
		printf("Failed to mount data volume (%d)\n", error);
	}

	vnode_put(rootvnode);

	return error;

#else
	return 0;
#endif
}

/*
 * Mount the VM volume of a container
 */
int
vfs_mount_vm(void)
{
#if CONFIG_MOUNT_VM
	int error = 0;

	error = vnode_get(rootvnode);
	if (error) {
		/* root must be mounted first */
		printf("vnode_get(rootvnode) failed with error %d\n", error);
		return error;
	}

	char vmpath[] = PLATFORM_VM_VOLUME_MOUNT_POINT; /* !const because of internal casting */

	/* Mount the VM volume */
	printf("attempting kernel mount for vm volume... \n");
	error = kernel_mount(rootvnode->v_mount->mnt_vfsstat.f_fstypename, NULLVP, NULLVP,
	    vmpath, (rootvnode->v_mount), 0, 0, (KERNEL_MOUNT_VMVOL), vfs_context_kernel());

	if (error) {
		printf("Failed to mount vm volume (%d)\n", error);
	} else {
		printf("mounted VM volume\n");
	}

	vnode_put(rootvnode);
	return error;
#else
	return 0;
#endif
}

/*
 * Lookup a mount point by filesystem identifier.
 */

struct mount *
vfs_getvfs(fsid_t *fsid)
{
	return mount_list_lookupby_fsid(fsid, 0, 0);
}

static struct mount *
vfs_getvfs_locked(fsid_t *fsid)
{
	return mount_list_lookupby_fsid(fsid, 1, 0);
}

struct mount *
vfs_getvfs_by_mntonname(char *path)
	mount_t retmp = (mount_t)0;
	mount_t mp;

	mount_list_lock();
	TAILQ_FOREACH(mp, &mountlist, mnt_list) {
		if (!strncmp(mp->mnt_vfsstat.f_mntonname, path,
		    sizeof(mp->mnt_vfsstat.f_mntonname))) {
			retmp = mp;
			if (mount_iterref(retmp, 1)) {
				retmp = NULL;
			}
			goto out;
		}
	}
out:
	mount_list_unlock();
	return retmp;
}
/* generation number for creation of new fsids */
u_short mntid_gen = 0;
/*
 * Get a new unique fsid
 */
void
vfs_getnewfsid(struct mount *mp)
{
	if (++mntid_gen == 0) {
	}
	while (vfs_getvfs_locked(&tfsid)) {
		if (++mntid_gen == 0) {
			mntid_gen++;
		tfsid.val[0] = makedev(nblkdev + mtype, mntid_gen);

extern int(**dead_vnodeop_p)(void *);
	if ((lmp = vp->v_mount) != NULL && lmp != dead_mountp) {
		if ((vp->v_lflag & VNAMED_MOUNT) == 0) {
		}
			if (TAILQ_LAST(&lmp->mnt_vnodelist, vnodelst) == vp) {
			} else if (TAILQ_LAST(&lmp->mnt_newvnodes, vnodelst) == vp) {
			} else if (TAILQ_LAST(&lmp->mnt_workerqueue, vnodelst) == vp) {
			}
		} else {
		}
		if ((vp->v_mntvnodes.tqe_next != 0) && (vp->v_mntvnodes.tqe_prev != 0)) {
		}
		if (mp->mnt_lflag & MNT_LITER) {
		} else {
		}
		if (vp->v_lflag & VNAMED_MOUNT) {
		}
	vnode_t nvp;
	int     error;
		return ENODEV;
	if ((error = vnode_create(VNCREATE_FLAVOR, VCREATESIZE, &vfsp, &nvp))) {
		return error;
	nvp->v_tag = VT_NON;    /* set this to VT_NON so during aliasing it can be replaced */
	if ((error = vnode_ref(nvp))) {
		return error;
	if ((error = VNOP_FSYNC(nvp, MNT_WAIT, &context))) {
		return error;
	if ((error = buf_invalidateblks(nvp, BUF_WRITE_DATA, 0, 0))) {
		return error;
	/*
#endif /* MAC */
	if ((error = VNOP_OPEN(nvp, FREAD, &context))) {
		return error;
	return 0;
			vid = vp->v_id;
		if (vnode_getwithvid(vp, vid)) {
			goto loop;
			vnode_reclaim_internal(vp, 1, 1, 0);
			    M_SPECINFO, M_WAITOK);

		}
		return NULLVP;
	if ((vp->v_flag & (VBDEVVP | VDEVFLUSH)) != 0) {
		return vp;
	}
	return vp;
	if ((vflags & VNODE_WRITEABLE) && (vp->v_writecount == 0)) {
		/*
		 * vnode to be returned only if it has writers opened
		error = EINVAL;
	} else {
		error = vnode_getiocount(vp, vid, vflags);
	}
	return error;
	return vnode_ref_ext(vp, 0, 0);
	int     error = 0;
	if (vp->v_iocount <= 0 && vp->v_usecount <= 0) {
	}
		if (++vp->v_writecount <= 0) {
			panic("vnode_ref_ext: v_writecount");
		}
		if (++vp->v_kusecount <= 0) {
			panic("vnode_ref_ext: v_kusecount");
		}
		struct  uthread *ut;
		ut = get_bsdthread_info(current_thread());

		if (!(current_proc()->p_lflag & P_LRAGE_VNODES) &&
		    !(ut->uu_flag & UT_RAGE_VNODES)) {
			/*
	return error;
boolean_t
	if (!(vp->v_mount->mnt_kern_flag & MNTK_VIRTUALDEV) && (vp->v_mount->mnt_flag & MNT_LOCAL)) {
		return TRUE;
	}
	return FALSE;
	if (VONLIST(vp) || (vp->v_lflag & (VL_TERMINATE | VL_DEAD))) {
	}

again:

	 * if it is already on a list or non zero references return
	if (VONLIST(vp) || (vp->v_usecount != 0) || (vp->v_iocount != 0) || (vp->v_lflag & VL_TERMINATE)) {
	}

	/*
	 * In vclean, we might have deferred ditching locked buffers
	 * because something was still referencing them (indicated by
	 * usecount).  We can ditch them now.
	 */
	if (ISSET(vp->v_lflag, VL_DEAD)
	    && (!LIST_EMPTY(&vp->v_cleanblkhd) || !LIST_EMPTY(&vp->v_dirtyblkhd))) {
		++vp->v_iocount;        // Probably not necessary, but harmless
#ifdef JOE_DEBUG
		record_vp(vp, 1);
#endif
		vnode_unlock(vp);
		buf_invalidateblks(vp, BUF_INVALIDATE_LOCKED, 0, 0);
		vnode_lock(vp);
		vnode_dropiocount(vp);
		goto again;
	}
		if ((vp->v_flag & VAGE)) {
			TAILQ_INSERT_HEAD(&vnode_rage_list, vp, v_freelist);
		} else {
			TAILQ_INSERT_TAIL(&vnode_rage_list, vp, v_freelist);
		}
		/*
		if ((vp->v_lflag & VL_DEAD)) {
			TAILQ_INSERT_HEAD(&vnode_dead_list, vp, v_freelist);
		} else if ((vp->v_flag & VAGE)) {
			TAILQ_INSERT_HEAD(&vnode_free_list, vp, v_freelist);
			TAILQ_INSERT_TAIL(&vnode_free_list, vp, v_freelist);
	if (need_dead_wakeup == TRUE) {
	}
		if (vp->v_listflag & VLIST_RAGE) {
			VREMRAGE("vnode_list_remove", vp);
		} else if (vp->v_listflag & VLIST_DEAD) {
			VREMDEAD("vnode_list_remove", vp);
		} else if (vp->v_listflag & VLIST_ASYNC_WORK) {
			VREMASYNC_WORK("vnode_list_remove", vp);
		} else {
			VREMFREE("vnode_list_remove", vp);
		}
	/*
	 * the not-on-list state to the on-list
		vnode_list_lock();
		 * hold the vnode_list_lock... this
	vnode_rele_internal(vp, 0, 0, 0);
	vnode_rele_internal(vp, fmode, dont_reenter, 0);
	if (!locked) {
		vnode_lock_spin(vp);
	}
	else {
	}
	if (--vp->v_usecount < 0) {
		panic("vnode_rele_ext: vp %p usecount -ve : %d.  v_tag = %d, v_type = %d, v_flag = %x.", vp, vp->v_usecount, vp->v_tag, vp->v_type, vp->v_flag);
	}
		if (--vp->v_writecount < 0) {
			panic("vnode_rele_ext: vp %p writecount -ve : %d.  v_tag = %d, v_type = %d, v_flag = %x.", vp, vp->v_writecount, vp->v_tag, vp->v_type, vp->v_flag);
		}
		if (--vp->v_kusecount < 0) {
			panic("vnode_rele_ext: vp %p kusecount -ve : %d.  v_tag = %d, v_type = %d, v_flag = %x.", vp, vp->v_kusecount, vp->v_tag, vp->v_type, vp->v_flag);
		}
	}
	if (vp->v_kusecount > vp->v_usecount) {
		panic("vnode_rele_ext: vp %p kusecount(%d) out of balance with usecount(%d).  v_tag = %d, v_type = %d, v_flag = %x.", vp, vp->v_kusecount, vp->v_usecount, vp->v_tag, vp->v_type, vp->v_flag);
		/*
		if (vp->v_usecount == 0) {
			vp->v_lflag |= VL_NEEDINACTIVE;
	if (ISSET(vp->v_lflag, VL_TERMINATE | VL_DEAD) || dont_reenter) {
		/*
		 * the filesystem on this release...in
		 * the latter case, we'll mark the vnode aged
		if (dont_reenter) {
			if (!(vp->v_lflag & (VL_TERMINATE | VL_DEAD | VL_MARKTERM))) {
				vp->v_lflag |= VL_NEEDINACTIVE;

				if (vnode_on_reliable_media(vp) == FALSE || vp->v_flag & VISDIRTY) {
		vnode_list_add(vp);
	vp->v_lflag &= ~VL_NEEDINACTIVE;
	if ((vp->v_iocount == 1) && (vp->v_usecount == 0) &&
	    ((vp->v_lflag & (VL_MARKTERM | VL_TERMINATE | VL_DEAD)) == VL_MARKTERM)) {
		struct  uthread *ut;

		ut = get_bsdthread_info(current_thread());
			vp->v_defer_reclaimlist = ut->uu_vreclaims;
		vnode_reclaim_internal(vp, 1, 1, 0);
	if (!locked) {
		vnode_unlock(vp);
	}
	bool first_try = true;

	/*
	 * See comments in vnode_iterate() for the rationale for this lock
	 */
	mount_iterate_lock(mp);
	if (((flags & FORCECLOSE) == 0) && ((mp->mnt_kern_flag & MNTK_UNMOUNT_PREFLIGHT) != 0)) {
			mount_iterate_unlock(mp);
			return EBUSY;
	/* If it returns 0 then there is nothing to do */
	if (retval == 0) {
		mount_iterate_unlock(mp);
		return retval;
		if ((vp->v_mount != mp) || (vp == skipvp)) {
		// If vnode is already terminating, wait for it...
		while (vp->v_id == vid && ISSET(vp->v_lflag, VL_TERMINATE)) {
			vp->v_lflag |= VL_TERMWANT;
			msleep(&vp->v_lflag, &vp->v_lock, PVFS, "vflush", NULL);
		}

		if ((vp->v_id != vid) || ISSET(vp->v_lflag, VL_DEAD)) {
			vnode_unlock(vp);
			mount_lock(mp);
			continue;
		 */
			vp->v_iocount++;        /* so that drain waits for * other iocounts */
				vp->v_iocount++;        /* so that drain waits * for other iocounts */

		/* log vnodes blocking unforced unmounts */
		if (print_busy_vnodes && first_try && ((flags & FORCECLOSE) == 0)) {
			vprint("vflush - busy vnode", vp);
		}

	if (busy && ((flags & FORCECLOSE) == 0) && reclaimed) {
		first_try = false;
	if (vnode_iterate_reloadq(mp) != 0) {
		if (!(busy && ((flags & FORCECLOSE) == 0))) {
			first_try = false;
		}
	mount_iterate_unlock(mp);
	if (busy && ((flags & FORCECLOSE) == 0)) {
		return EBUSY;
	}
	return 0;
	if (flags & DOCLOSE) {
	}
	if (flags & REVOKEALL) {
	}

	if (active && (flags & DOCLOSE)) {
	}
		if (vp->v_tag == VT_NFS) {
		} else
			VNOP_FSYNC(vp, MNT_WAIT, ctx);

			/*
			 * If the vnode is still in use (by the journal for
			 * example) we don't want to invalidate locked buffers
			 * here.  In that case, either the journal will tidy them
			 * up, or we will deal with it when the usecount is
			 * finally released in vnode_rele_internal.
			 */
			buf_invalidateblks(vp, BUF_WRITE_DATA | (active ? 0 : BUF_INVALIDATE_LOCKED), 0, 0);
		if (UBCINFOEXISTS(vp)) {
			/*
			(void)ubc_msync(vp, (off_t)0, ubc_getsize(vp), NULL, UBC_PUSHALL | UBC_INVALIDATE | UBC_SYNC);
		}
	if (active || need_inactive) {
	}

			vnode_relenamedstream(pvp, vp);

		/*
	 * (and in the case of forced unmount of an mmap-ed file,
	 * the ubc reference on the vnode is dropped here too).
	if (vp->v_resolve) {
	}
	if (VNOP_RECLAIM(vp, ctx)) {
	}

	vnode_update_identity(vp, NULLVP, NULL, 0, 0, VNODE_UPDATE_PARENT | VNODE_UPDATE_NAME | VNODE_UPDATE_PURGE | VNODE_UPDATE_PURGEFIRMLINK);
	/*
	 * Remove the vnode from any mount list it might be on.  It is not
	 * safe to do this any earlier because unmount needs to wait for
	 * any vnodes to terminate and it cannot do that if it cannot find
	 * them.
	 */
	insmntque(vp, (struct mount *)0);

	vp->v_flag &= ~VISDIRTY;
		vp->v_lflag &= ~VL_TERMINATE;
			vp->v_lflag &= ~VL_TERMWANT;
	if ((flags & REVOKEALL) == 0) {
	}
		if (vp->v_lflag & VL_TERMINATE) {
			return ENOENT;
		}
				    vq->v_type != vp->v_type || vp == vq) {
				}
				if (vnode_getwithvid(vq, vid)) {
					SPECHASH_LOCK();
				vnode_lock(vq);
				if (!(vq->v_lflag & VL_TERMINATE)) {
					vnode_reclaim_internal(vq, 1, 1, 0);
				}
				vnode_put_locked(vq);
				vnode_unlock(vq);
	vnode_lock(vp);
	if (vp->v_lflag & VL_TERMINATE) {
		vnode_unlock(vp);
		return ENOENT;
	}
	vnode_reclaim_internal(vp, 1, 0, REVOKEALL);
	vnode_unlock(vp);
	return 0;
		return 0;
	}
	return 1;
		return 0;
	}
	if (vp->v_iocount <= 0) {
	}
	return 1;
		SPECHASH_LOCK();
		if (*vp->v_hashchain == vp) {
			*vp->v_hashchain = vp->v_specnext;
		} else {
			for (vq = *vp->v_hashchain; vq; vq = vq->v_specnext) {
				if (vq->v_specnext != vp) {
					continue;
				vq->v_specnext = vp->v_specnext;
				break;
			}
			if (vq == NULL) {
		}
		if (vp->v_specflags & SI_ALIASED) {
			vx = NULL;
			for (vq = *vp->v_hashchain; vq; vq = vq->v_specnext) {
				if (vq->v_rdev != vp->v_rdev ||
				    vq->v_type != vp->v_type) {
					continue;
				if (vx) {
					break;
				}
				vx = vq;
			if (vx == NULL) {
				panic("missing alias");
			}
			if (vq == NULL) {
				vx->v_specflags &= ~SI_ALIASED;
			}
			vp->v_specflags &= ~SI_ALIASED;
		}
		SPECHASH_UNLOCK();
		{
			FREE_ZONE(tmp, sizeof(struct specinfo), M_SPECINFO);
		}
	vnode_t vp;
		if (dev != vp->v_rdev || type != vp->v_type) {
		}
		if (vnode_getwithvid(vp, vid)) {
		}
			if ((*errorp = vfs_mountedon(vp)) != 0) {
			}
		} else {
			vnode_unlock(vp);
		}
		return rc;
	return 0;
	if (!vnode_isspec(vp)) {
		return vp->v_usecount - vp->v_kusecount;
	}

	if (!vnode_isaliased(vp)) {
		return vp->v_specinfo->si_opencount;
	}
			if ((vq->v_usecount == 0) && (vq->v_iocount == 1) && vq != vp) {
	return count;
int     prtactive = 0;          /* 1 => print out reclaim of active vnodes */
{ "VNON", "VREG", "VDIR", "VBLK", "VCHR", "VLNK", "VSOCK", "VFIFO", "VBAD" };
	if (label != NULL) {
	}
	printf("name %s type %s, usecount %d, writecount %d\n",
	    vp->v_name, typename[vp->v_type],
	    vp->v_usecount, vp->v_writecount);
	if (vp->v_flag & VROOT) {
	}
	if (vp->v_flag & VTEXT) {
	}
	if (vp->v_flag & VSYSTEM) {
	}
	if (vp->v_flag & VNOFLUSH) {
	}
	if (vp->v_flag & VBWAIT) {
	}
	if (vnode_isaliased(vp)) {
	}
	if (sbuf[0] != '\0') {
		printf("vnode flags (%s\n", &sbuf[1]);
	}
/*
 * vn_getpath_fsenter_with_parent will reenter the file system to fine the path of the
 * vnode.  It requires that there are IO counts on both the vnode and the directory vnode.
 *
 * vn_getpath_fsenter is called by MAC hooks to authorize operations for every thing, but
 * unlink, rmdir and rename. For these operation the MAC hook  calls vn_getpath. This presents
 * problems where if the path can not be found from the name cache, those operations can
 * erroneously fail with EPERM even though the call should succeed. When removing or moving
 * file system objects with operations such as unlink or rename, those operations need to
 * take IO counts on the target and containing directory. Calling vn_getpath_fsenter from a
 * MAC hook from these operations during forced unmount operations can lead to dead
 * lock. This happens when the operation starts, IO counts are taken on the containing
 * directories and targets. Before the MAC hook is called a forced unmount from another
 * thread takes place and blocks on the on going operation's directory vnode in vdrain.
 * After which, the MAC hook gets called and calls vn_getpath_fsenter.  vn_getpath_fsenter
 * is called with the understanding that there is an IO count on the target. If in
 * build_path the directory vnode is no longer in the cache, then the parent object id via
 * vnode_getattr from the target is obtain and used to call VFS_VGET to get the parent
 * vnode. The file system's VFS_VGET then looks up by inode in its hash and tries to get
 * an IO count. But VFS_VGET "sees" the directory vnode is in vdrain and can block
 * depending on which version and how it calls the vnode_get family of interfaces.
 *
 * N.B.  A reasonable interface to use is vnode_getwithvid. This interface was modified to
 * call vnode_getiocount with VNODE_DRAINO, so it will happily get an IO count and not
 * cause issues, but there is no guarantee that all or any file systems are doing that.
 *
 * vn_getpath_fsenter_with_parent can enter the file system safely since there is a known
 * IO count on the directory vnode by calling build_path_with_parent.
 */

int
vn_getpath_fsenter_with_parent(struct vnode *dvp, struct vnode *vp, char *pathbuf, int *len)
{
	return build_path_with_parent(vp, dvp, pathbuf, *len, len, 0, vfs_context_current());
}

int
vn_getpath_ext(struct vnode *vp, struct vnode *dvp, char *pathbuf, int *len, int flags)
{
	int bpflags = (flags & VN_GETPATH_FSENTER) ? 0 : BUILDPATH_NO_FS_ENTER;

	if (flags && (flags != VN_GETPATH_FSENTER)) {
		if (flags & VN_GETPATH_NO_FIRMLINK) {
			bpflags |= BUILDPATH_NO_FIRMLINK;;
		}
		if (flags & VN_GETPATH_VOLUME_RELATIVE) {
			bpflags |= (BUILDPATH_VOLUME_RELATIVE | BUILDPATH_NO_FIRMLINK);
		}
	}

	return build_path_with_parent(vp, dvp, pathbuf, *len, len, bpflags, vfs_context_current());
}

int
vn_getpath_no_firmlink(struct vnode *vp, char *pathbuf, int *len)
{
	return vn_getpath_ext(vp, NULLVP, pathbuf, len, VN_GETPATH_NO_FIRMLINK);
}

static char *extension_table = NULL;
	return strlen((const char *)a) - strlen((const char *)b);
	char *new_exts, *old_exts;
	int error;

	if (nentries <= 0 || nentries > 1024 || maxwidth <= 0 || maxwidth > 255) {
		return EINVAL;
	}

	// allocate one byte extra so we can guarantee null termination
	MALLOC(new_exts, char *, (nentries * maxwidth) + 1, M_TEMP, M_WAITOK);
	if (new_exts == NULL) {
		return ENOMEM;
	}

	error = copyin(data, new_exts, nentries * maxwidth);
	if (error) {
		FREE(new_exts, M_TEMP);
		return error;
	}
	new_exts[(nentries * maxwidth)] = '\0'; // guarantee null termination of the block
	qsort(new_exts, nentries, maxwidth, extension_cmp);
	lck_mtx_lock(pkg_extensions_lck);
	old_exts        = extension_table;
	extension_table = new_exts;
	nexts           = nentries;
	max_ext_width   = maxwidth;
	lck_mtx_unlock(pkg_extensions_lck);
	if (old_exts) {
		FREE(old_exts, M_TEMP);
	}

	return 0;
int
	int i, extlen;
	const char *ptr, *name_ext;
	if (len <= 3) {
		return 0;
	name_ext = NULL;
	for (ptr = name; *ptr != '\0'; ptr++) {
		if (*ptr == '.') {
			name_ext = ptr;
		}
	}
	// if there is no "." extension, it can't match
	if (name_ext == NULL) {
		return 0;
	}

	// advance over the "."
	name_ext++;
	lck_mtx_lock(pkg_extensions_lck);
	// now iterate over all the extensions to see if any match
	ptr = &extension_table[0];
	for (i = 0; i < nexts; i++, ptr += max_ext_width) {
		extlen = strlen(ptr);
		if (strncasecmp(name_ext, ptr, extlen) == 0 && name_ext[extlen] == '\0') {
			// aha, a match!
			lck_mtx_unlock(pkg_extensions_lck);
			return 1;
		}
	lck_mtx_unlock(pkg_extensions_lck);
	// if we get here, no extension matched
	return 0;
	char *ptr, *end;
	int comp = 0;
	*component = -1;
	if (*path != '/') {
		return EINVAL;
	end = path + 1;
	while (end < path + pathlen && *end != '\0') {
		while (end < path + pathlen && *end == '/' && *end != '\0') {
			end++;
		}
		ptr = end;
		while (end < path + pathlen && *end != '/' && *end != '\0') {
			end++;
		}
		if (end > path + pathlen) {
			// hmm, string wasn't null terminated
			return EINVAL;
		}

		*end = '\0';
		if (is_package_name(ptr, end - ptr)) {
			*component = comp;
			break;
		}
		end++;
		comp++;
	}
	return 0;
/*
int
vn_searchfs_inappropriate_name(const char *name, int len)
{
	for (i = 0; i < (int) (sizeof(bad_names) / sizeof(bad_names[0])); i++) {
/*
 * The VFS_NUMMNTOPS shouldn't be at name[1] since
 * is a VFS generic variable. Since we no longer support
 * VT_UFS, we reserve its value to support this sysctl node.
 *
 * It should have been:
 *    name[0]:  VFS_GENERIC
 *    name[1]:  VFS_NUMMNTOPS
 */
SYSCTL_INT(_vfs, VFS_NUMMNTOPS, nummntops,
    CTLFLAG_RD | CTLFLAG_KERN | CTLFLAG_LOCKED,
    &vfs_nummntops, 0, "");

vfs_sysctl(int *name __unused, u_int namelen __unused,
    user_addr_t oldp __unused, size_t *oldlenp __unused,
    user_addr_t newp __unused, size_t newlen __unused, proc_t p __unused);

int
vfs_sysctl(int *name __unused, u_int namelen __unused,
    user_addr_t oldp __unused, size_t *oldlenp __unused,
    user_addr_t newp __unused, size_t newlen __unused, proc_t p __unused)
	return EINVAL;
}
//
// The following code disallows specific sysctl's that came through
// the direct sysctl interface (vfs_sysctl_node) instead of the newer
// sysctl_vfs_ctlbyfsid() interface.  We can not allow these selectors
// through vfs_sysctl_node() because it passes the user's oldp pointer
// directly to the file system which (for these selectors) casts it
// back to a struct sysctl_req and then proceed to use SYSCTL_IN()
// which jumps through an arbitrary function pointer.  When called
// through the sysctl_vfs_ctlbyfsid() interface this does not happen
// and so it's safe.
//
// Unfortunately we have to pull in definitions from AFP and SMB and
// perform explicit name checks on the file system to determine if
// these selectors are being used.
//
#define AFPFS_VFS_CTL_GETID            0x00020001
#define AFPFS_VFS_CTL_NETCHANGE        0x00020002
#define AFPFS_VFS_CTL_VOLCHANGE        0x00020003
#define SMBFS_SYSCTL_REMOUNT           1
#define SMBFS_SYSCTL_REMOUNT_INFO      2
#define SMBFS_SYSCTL_GET_SERVER_SHARE  3
static int
is_bad_sysctl_name(struct vfstable *vfsp, int selector_name)
{
	switch (selector_name) {
	case VFS_CTL_QUERY:
	case VFS_CTL_TIMEO:
	case VFS_CTL_NOLOCKS:
	case VFS_CTL_NSTATUS:
	case VFS_CTL_SADDR:
	case VFS_CTL_DISC:
	case VFS_CTL_SERVERINFO:
		return 1;
	default:
		break;
	// the more complicated check for some of SMB's special values
	if (strcmp(vfsp->vfc_name, "smbfs") == 0) {
		switch (selector_name) {
		case SMBFS_SYSCTL_REMOUNT:
		case SMBFS_SYSCTL_REMOUNT_INFO:
		case SMBFS_SYSCTL_GET_SERVER_SHARE:
			return 1;
		}
	} else if (strcmp(vfsp->vfc_name, "afpfs") == 0) {
		switch (selector_name) {
		case AFPFS_VFS_CTL_GETID:
		case AFPFS_VFS_CTL_NETCHANGE:
		case AFPFS_VFS_CTL_VOLCHANGE:
			return 1;
	//
	// If we get here we passed all the checks so the selector is ok
	//
	return 0;

int vfs_sysctl_node SYSCTL_HANDLER_ARGS
	int *name, namelen;
	struct vfstable *vfsp;
	int error;
	int fstypenum;
	fstypenum = oidp->oid_number;
	name = arg1;
	namelen = arg2;
	/* all sysctl names at this level should have at least one name slot for the FS */
	if (namelen < 1) {
		return EISDIR; /* overloaded */
	}
	mount_list_lock();
	for (vfsp = vfsconf; vfsp; vfsp = vfsp->vfc_next) {
		if (vfsp->vfc_typenum == fstypenum) {
			vfsp->vfc_refcount++;
			break;
		}
	}
	mount_list_unlock();

	if (vfsp == NULL) {
		return ENOTSUP;
	}

	if (is_bad_sysctl_name(vfsp, name[0])) {
		printf("vfs: bad selector 0x%.8x for old-style sysctl().  use the sysctl-by-fsid interface instead\n", name[0]);
		return EPERM;
	}

	error = (vfsp->vfc_vfsops->vfs_sysctl)(name, namelen, req->oldptr, &req->oldlen, req->newptr, req->newlen, vfs_context_current());

	mount_list_lock();
	vfsp->vfc_refcount--;
	mount_list_unlock();

	return error;
}
			    vq->v_type != vp->v_type) {
			}
	return error;
}

struct unmount_info {
	int     u_errs; // Total failed unmounts
	int     u_busy; // EBUSY failed unmounts
};

static int
unmount_callback(mount_t mp, void *arg)
{
	int error;
	char *mntname;
	struct unmount_info *uip = arg;

	mount_ref(mp, 0);
	mount_iterdrop(mp);     // avoid vfs_iterate deadlock in dounmount()

	MALLOC_ZONE(mntname, void *, MAXPATHLEN, M_NAMEI, M_WAITOK);
	if (mntname) {
		strlcpy(mntname, mp->mnt_vfsstat.f_mntonname, MAXPATHLEN);
	}

	error = dounmount(mp, MNT_FORCE, 1, vfs_context_current());
	if (error) {
		uip->u_errs++;
		printf("Unmount of %s failed (%d)\n", mntname ? mntname:"?", error);
		if (error == EBUSY) {
			uip->u_busy++;
		}
	}
	if (mntname) {
		FREE_ZONE(mntname, MAXPATHLEN, M_NAMEI);
	}

	return VFS_RETURNED;
 * Busy mounts are retried.
	int mounts, sec = 1;
	struct unmount_info ui;
retry:
	ui.u_errs = ui.u_busy = 0;
	vfs_iterate(VFS_ITERATE_CB_DROPREF | VFS_ITERATE_TAIL_FIRST, unmount_callback, &ui);
	mounts = mount_getvfscnt();
	if (mounts == 0) {
		return;
	if (ui.u_busy > 0) {            // Busy mounts - wait & retry
		tsleep(&nummounts, PVFS, "busy mount", sec * hz);
		sec *= 2;
		if (sec <= 32) {
			goto retry;
		}
		printf("Unmounting timed out\n");
	} else if (ui.u_errs < mounts) {
		// If the vfs_iterate missed mounts in progress - wait a bit
		tsleep(&nummounts, PVFS, "missed mount", 2 * hz);
	}
}
/*
	struct ubc_info *uip;
	if (vp->v_usecount != 0) {
		/*
		 * At the eleventh hour, just before the ubcinfo is
		 * destroyed, ensure the ubc-specific v_usecount
		 * reference has gone.  We use v_usecount != 0 as a hint;
		 * ubc_unmap() does nothing if there's no mapping.
		 *
		 * This case is caused by coming here via forced unmount,
		 * versus the usual vm_object_deallocate() path.
		 * In the forced unmount case, ubc_destroy_named()
		 * releases the pager before memory_object_last_unmap()
		 * can be called.
		 */
		vnode_unlock(vp);
		ubc_unmap(vp);
		vnode_lock_spin(vp);
	}
#if CONFIG_IOSCHED
extern int lowpri_throttle_enabled;
extern int iosched_enabled;
#endif

	int     error;
	off_t   readblockcnt = 0;
	off_t   writeblockcnt = 0;
	off_t   readmaxcnt = 0;
	off_t   writemaxcnt = 0;
	off_t   readsegcnt = 0;
	off_t   writesegcnt = 0;
	off_t   readsegsize = 0;
	off_t   writesegsize = 0;
	off_t   alignment = 0;
	u_int32_t minsaturationbytecount = 0;
	u_int32_t ioqueue_depth = 0;
	u_int64_t location = 0;
	dk_corestorage_info_t cs_info;
	boolean_t cs_present = FALSE;;
	if (devvp == rootvp) {
	}
	    (caddr_t)&blksize, 0, ctx))) {
		return error;
	}
	mp->mnt_maxreadcnt = MAX_UPL_SIZE_BYTES;
	mp->mnt_maxwritecnt = MAX_UPL_SIZE_BYTES;
		if (isvirtual) {
			mp->mnt_kern_flag |= MNTK_VIRTUALDEV;
		}
		if (isssd) {
			mp->mnt_kern_flag |= MNTK_SSD;
		}
	    (caddr_t)&features, 0, ctx))) {
		return error;
	}
	    (caddr_t)&readblockcnt, 0, ctx))) {
		return error;
	}
	    (caddr_t)&writeblockcnt, 0, ctx))) {
		return error;
	}
	    (caddr_t)&readmaxcnt, 0, ctx))) {
		return error;
	}
	    (caddr_t)&writemaxcnt, 0, ctx))) {
		return error;
	}
	    (caddr_t)&readsegcnt, 0, ctx))) {
		return error;
	}
	    (caddr_t)&writesegcnt, 0, ctx))) {
		return error;
	}
	    (caddr_t)&readsegsize, 0, ctx))) {
		return error;
	}
	    (caddr_t)&writesegsize, 0, ctx))) {
		return error;
	}
	    (caddr_t)&alignment, 0, ctx))) {
		return error;
	}
	    (caddr_t)&ioqueue_depth, 0, ctx))) {
		return error;
	}
	if (readmaxcnt) {
	}
		if (temp < mp->mnt_maxreadcnt) {
		}
	if (writemaxcnt) {
	}
		if (temp < mp->mnt_maxwritecnt) {
		}
		temp = (readsegcnt > UINT16_MAX) ? UINT16_MAX : readsegcnt;
		if (temp > UINT16_MAX) {
		}
		temp = (writesegcnt > UINT16_MAX) ? UINT16_MAX : writesegcnt;
		if (temp > UINT16_MAX) {
		}
	if (readsegsize) {
		temp = (readsegsize > UINT32_MAX) ? UINT32_MAX : readsegsize;
	} else {
		temp = mp->mnt_maxreadcnt;
	}
	if (writesegsize) {
		temp = (writesegsize > UINT32_MAX) ? UINT32_MAX : writesegsize;
	} else {
		temp = mp->mnt_maxwritecnt;
	}
	if (alignment) {
		temp = (alignment > PAGE_SIZE) ? PAGE_MASK : alignment - 1;
	} else {
		temp = 0;
	}
	if (ioqueue_depth > MNT_DEFAULT_IOQUEUE_DEPTH) {
	} else {
	}
	mp->mnt_ioscale = MNT_IOSCALE(mp->mnt_ioqueue_depth);
	if (mp->mnt_ioscale > 1) {
	}

	if (features & DK_FEATURE_FORCE_UNIT_ACCESS) {
		mp->mnt_ioflags |= MNT_IOFLAGS_FUA_SUPPORTED;
	}
	if (VNOP_IOCTL(devvp, DKIOCGETIOMINSATURATIONBYTECOUNT, (caddr_t)&minsaturationbytecount, 0, ctx) == 0) {
		mp->mnt_minsaturationbytecount = minsaturationbytecount;
	} else {
		mp->mnt_minsaturationbytecount = 0;
	}

	if (VNOP_IOCTL(devvp, DKIOCCORESTORAGE, (caddr_t)&cs_info, 0, ctx) == 0) {
		cs_present = TRUE;
	}

	if (features & DK_FEATURE_UNMAP) {

		if (cs_present == TRUE) {
			mp->mnt_ioflags |= MNT_IOFLAGS_CSUNMAP_SUPPORTED;
		}
	}
	if (cs_present == TRUE) {
		/*
		 * for now we'll use the following test as a proxy for
		 * the underlying drive being FUSION in nature
		 */
		if ((cs_info.flags & DK_CORESTORAGE_PIN_YOUR_METADATA)) {
			mp->mnt_ioflags |= MNT_IOFLAGS_FUSION_DRIVE;
		}
	} else {
		/* Check for APFS Fusion */
		dk_apfs_flavour_t flavour;
		if ((VNOP_IOCTL(devvp, DKIOCGETAPFSFLAVOUR, (caddr_t)&flavour, 0, ctx) == 0) &&
		    (flavour == DK_APFS_FUSION)) {
			mp->mnt_ioflags |= MNT_IOFLAGS_FUSION_DRIVE;
		}
	}

	if (VNOP_IOCTL(devvp, DKIOCGETLOCATION, (caddr_t)&location, 0, ctx) == 0) {
		if (location & DK_LOCATION_EXTERNAL) {
			mp->mnt_ioflags |= MNT_IOFLAGS_PERIPHERAL_DRIVE;
			/* This must be called after MNTK_VIRTUALDEV has been determined via DKIOCISVIRTUAL */
			if ((MNTK_VIRTUALDEV & mp->mnt_kern_flag)) {
				mp->mnt_flag |= MNT_REMOVABLE;
			}
		}
	}

#if CONFIG_IOSCHED
	if (iosched_enabled && (features & DK_FEATURE_PRIORITY)) {
		mp->mnt_ioflags |= MNT_IOFLAGS_IOSCHED_SUPPORTED;
		throttle_info_disable_throttle(mp->mnt_devbsdunit, (mp->mnt_ioflags & MNT_IOFLAGS_FUSION_DRIVE) != 0);
	}
#endif /* CONFIG_IOSCHED */
	return error;
			if (data) {
				mp->mnt_kern_flag &= ~MNT_LNOTRESP;     // Now responding
			} else {
				mp->mnt_kern_flag |= MNT_LNOTRESP;      // Not responding
			}
	return mount_getvfscnt();
	return ret;
	int actual = 0;
	return actual;
		if (*actual <= count) {
		}
	return *actual <= count ? 0 : ENOMEM;
    __unused int arg2, struct sysctl_req *req)
	if (req->newptr != USER_ADDR_NULL) {
		return EPERM;
	}
	if (space < req->oldlen) {
		return ENOMEM;
	}
	MALLOC(fsidlst, fsid_t *, req->oldlen, M_TEMP, M_WAITOK | M_ZERO);
		return ENOMEM;
	return error;
    struct sysctl_req *req)
	int error = 0, gotref = 0;
	proc_t p = req->p;      /* XXX req->p != current_proc()? */
	if (error) {
	}
				    NULL, USER_ADDR_NULL, 0,
			} else {
		} else {
			    NULL, USER_ADDR_NULL, 0,
		} else {
		if (error) {
		}
		} else {
		if (error) {
		}
		    (error = vfs_update_vfsstat(mp, ctx, VFS_USER_EVENT))) {
		}
#ifdef NFSCLIENT
				strlcpy(&sfs.f_fstypename[0], &mp->fstypename_override[0], MFSNAMELEN);
			} else
#endif
			{

		} else {
				int             shift;
					if ((sp->f_blocks >> shift) <= INT_MAX) {
					}
					if ((((long long)sp->f_bsize) << (shift + 1)) > INT_MAX) {
					}
#define __SHIFT_OR_CLIP(x, s)   ((((x) >> (s)) > INT_MAX) ? INT_MAX : ((x) >> (s)))

#ifdef NFSCLIENT
				strlcpy(&sfs.f_fstypename[0], &mp->fstypename_override[0], MFSNAMELEN);
			} else
#endif
			{

	if (gotref != 0) {
	}
	return error;
static int      filt_fsattach(struct knote *kn, struct kevent_qos_s *kev);
static void     filt_fsdetach(struct knote *kn);
static int      filt_fsevent(struct knote *kn, long hint);
static int      filt_fstouch(struct knote *kn, struct kevent_qos_s *kev);
static int      filt_fsprocess(struct knote *kn, struct kevent_qos_s *kev);
SECURITY_READ_ONLY_EARLY(struct filterops) fs_filtops = {
	.f_attach = filt_fsattach,
	.f_detach = filt_fsdetach,
	.f_event = filt_fsevent,
	.f_touch = filt_fstouch,
	.f_process = filt_fsprocess,
filt_fsattach(struct knote *kn, __unused struct kevent_qos_s *kev)
	kn->kn_flags |= EV_CLEAR; /* automatic */
	kn->kn_sdata = 0;         /* incoming data is ignored */

	/*
	 * filter only sees future events,
	 * so it can't be fired already.
	 */
	return 0;
	return kn->kn_fflags != 0;
}

static int
filt_fstouch(struct knote *kn, struct kevent_qos_s *kev)
{
	int res;

	lck_mtx_lock(fs_klist_lock);

	kn->kn_sfflags = kev->fflags;

	/*
	 * the above filter function sets bits even if nobody is looking for them.
	 * Just preserve those bits even in the new mask is more selective
	 * than before.
	 *
	 * For compatibility with previous implementations, we leave kn_fflags
	 * as they were before.
	 */
	//if (kn->kn_sfflags)
	//	kn->kn_fflags &= kn->kn_sfflags;
	res = (kn->kn_fflags != 0);

	lck_mtx_unlock(fs_klist_lock);

	return res;
}

static int
filt_fsprocess(struct knote *kn, struct kevent_qos_s *kev)
{
	int res = 0;

	lck_mtx_lock(fs_klist_lock);
	if (kn->kn_fflags) {
		knote_fill_kevent(kn, kev, 0);
		res = 1;
	}
	lck_mtx_unlock(fs_klist_lock);
	return res;
    __unused void *arg1, __unused int arg2, struct sysctl_req *req)
	if (req->newptr == USER_ADDR_NULL) {
		return EINVAL;
	}
	if (error) {
		return error;
	}
	if (p == NULL) {
		return ESRCH;
	}
		return error;
		return EPERM;
	if (pid < 0) {
	} else {
	}
	return 0;
}

static int
sysctl_vfs_generic_conf SYSCTL_HANDLER_ARGS
{
	int *name, namelen;
	struct vfstable *vfsp;
	struct vfsconf vfsc = {};

	(void)oidp;
	name = arg1;
	namelen = arg2;

	if (namelen < 1) {
		return EISDIR;
	} else if (namelen > 1) {
		return ENOTDIR;
	}

	mount_list_lock();
	for (vfsp = vfsconf; vfsp; vfsp = vfsp->vfc_next) {
		if (vfsp->vfc_typenum == name[0]) {
			break;
		}
	}

	if (vfsp == NULL) {
		mount_list_unlock();
		return ENOTSUP;
	}

	vfsc.vfc_reserved1 = 0;
	bcopy(vfsp->vfc_name, vfsc.vfc_name, sizeof(vfsc.vfc_name));
	vfsc.vfc_typenum = vfsp->vfc_typenum;
	vfsc.vfc_refcount = vfsp->vfc_refcount;
	vfsc.vfc_flags = vfsp->vfc_flags;
	vfsc.vfc_reserved2 = 0;
	vfsc.vfc_reserved3 = 0;

	mount_list_unlock();
	return SYSCTL_OUT(req, &vfsc, sizeof(struct vfsconf));
SYSCTL_PROC(_vfs_generic, OID_AUTO, vfsidlist,
    CTLTYPE_STRUCT | CTLFLAG_RD | CTLFLAG_LOCKED,
SYSCTL_INT(_vfs_generic, VFS_MAXTYPENUM, maxtypenum,
    CTLFLAG_RD | CTLFLAG_KERN | CTLFLAG_LOCKED,
    &maxvfstypenum, 0, "");
SYSCTL_INT(_vfs_generic, OID_AUTO, sync_timeout, CTLFLAG_RW | CTLFLAG_LOCKED, &sync_timeout_seconds, 0, "");
SYSCTL_NODE(_vfs_generic, VFS_CONF, conf,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    sysctl_vfs_generic_conf, "");
#if DEVELOPMENT || DEBUG
SYSCTL_INT(_vfs_generic, OID_AUTO, print_busy_vnodes,
    CTLTYPE_INT | CTLFLAG_RW,
    &print_busy_vnodes, 0,
    "VFS log busy vnodes blocking unmount");
#endif

/* Indicate that the root file system unmounted cleanly */
static int vfs_root_unmounted_cleanly = 0;
SYSCTL_INT(_vfs_generic, OID_AUTO, root_unmounted_cleanly, CTLFLAG_RD, &vfs_root_unmounted_cleanly, 0, "Root filesystem was unmounted cleanly");

void
vfs_set_root_unmounted_cleanly(void)
{
	vfs_root_unmounted_cleanly = 1;
}

/*
 * Print vnode state.
 */
void
vn_print_state(struct vnode *vp, const char *fmt, ...)
{
	va_list ap;
	char perm_str[] = "(VM_KERNEL_ADDRPERM pointer)";
	char fs_name[MFSNAMELEN];

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf("vp 0x%0llx %s: ", (uint64_t)VM_KERNEL_ADDRPERM(vp), perm_str);
	printf("tag %d, type %d\n", vp->v_tag, vp->v_type);
	/* Counts .. */
	printf("    iocount %d, usecount %d, kusecount %d references %d\n",
	    vp->v_iocount, vp->v_usecount, vp->v_kusecount, vp->v_references);
	printf("    writecount %d, numoutput %d\n", vp->v_writecount,
	    vp->v_numoutput);
	/* Flags */
	printf("    flag 0x%x, lflag 0x%x, listflag 0x%x\n", vp->v_flag,
	    vp->v_lflag, vp->v_listflag);

	if (vp->v_mount == NULL || vp->v_mount == dead_mountp) {
		strlcpy(fs_name, "deadfs", MFSNAMELEN);
	} else {
		vfs_name(vp->v_mount, fs_name);
	}

	printf("    v_data 0x%0llx %s\n",
	    (vp->v_data ? (uint64_t)VM_KERNEL_ADDRPERM(vp->v_data) : 0),
	    perm_str);
	printf("    v_mount 0x%0llx %s vfs_name %s\n",
	    (vp->v_mount ? (uint64_t)VM_KERNEL_ADDRPERM(vp->v_mount) : 0),
	    perm_str, fs_name);
}

	/*
	 */
	if ((vpid != vp->v_id) || (vp->v_usecount != 0) || (vp->v_iocount != 0) ||

		return NULLVP;
	if ((vp->v_lflag & (VL_NEEDINACTIVE | VL_MARKTERM)) == VL_NEEDINACTIVE) {
		/*
		 * vnode_put will either leave us off
		vp->v_iocount++;
		return NULLVP;
	 */
		if (want_vp && (vnode_on_reliable_media(vp) == FALSE || (vp->v_flag & VISDIRTY))) {

			return NULLVP;
		if (vp->v_lflag & VL_DEAD) {
		}
			if ((VONLIST(vp))) {
			}
			    (vp->v_lflag & (VNAMED_UBC | VNAMED_MOUNT | VNAMED_FSHASH))) {
			}
			if ((vp->v_mntvnodes.tqe_prev != 0) && (vp->v_mntvnodes.tqe_next != 0)) {
			}
			if (!LIST_EMPTY(&vp->v_nclinks) || !TAILQ_EMPTY(&vp->v_ncchildren)) {
			}
	return vp;
__attribute__((noreturn))
	int     deferred;
	vnode_t vp;
		if (TAILQ_EMPTY(q)) {


		if (vp != NULLVP) {
		}
	vnode_t vp;
	uint32_t retries = 0, max_retries = 100;                /* retry incase of tablefull */
	struct timeval initial_tv;
	struct timeval current_tv;
	if (need_reliable_vp == TRUE) {
	}
		if (!TAILQ_EMPTY(&vnode_dead_list)) {
		VLISTNONE(vp);          /* avoid double queue removal */
		TAILQ_INIT(&vp->v_ncchildren);

		if (mac_vnode_label_init_needed(vp)) {
		}
	if (!TAILQ_EMPTY(&vnode_rage_list) &&
	    (ragevnodes >= rage_limit ||
	    (current_tv.tv_sec - rage_tv.tv_sec) >= RAGE_TIME_LIMIT)) {
			if (!(vp->v_listflag & VLIST_RAGE)) {
			}
			// may not be updated if there are multiple devnode layers
			if ((curproc->p_flag & P_DEPENDENCY_CAPABLE) == 0 || vp->v_mount == NULL ||
				 * mechanism to first scan for a reliable vnode before forcing
				if (need_reliable_vp == FALSE || vnode_on_reliable_media(vp) == TRUE) {
				}
		/*
			// may not be updated if there are multiple devnode layers
			if ((curproc->p_flag & P_DEPENDENCY_CAPABLE) == 0 || vp->v_mount == NULL ||
				 * mechanism to first scan for a reliable vnode before forcing
				if (need_reliable_vp == FALSE || vnode_on_reliable_media(vp) == TRUE) {
				}

		    "%d free, %d dead, %d async, %d rage\n",
		    desiredvnodes, numvnodes, freevnodes, deadvnodes, async_work_vnodes, ragevnodes);

#if DEVELOPMENT || DEBUG
		if (bootarg_no_vnode_jetsam) {
			panic("vnode table is full\n");
		}
#endif /* DEVELOPMENT || DEBUG */

		if (memorystatus_kill_on_vnode_limit() == FALSE) {
		/*
		 * Now that we've killed someone, wait a bit and continue looking
		retries = 0;
		return ENFILE;
			int     elapsed_msecs;
			if (initial_tv.tv_sec == 0) {
			}

				 * we've waited long enough... 100ms is
	assert((vp->v_lflag & VL_LABELWAIT) != VL_LABELWAIT);
	assert((vp->v_lflag & VL_LABEL) != VL_LABEL);
	if (vp->v_lflag & VL_LABELED || vp->v_label != NULL) {
		vnode_lock_convert(vp);
		vnode_lock_convert(vp);
	vp->v_references = 0;
	return 0;
	int retval;
	vnode_lock_spin(vp);
	return retval;
		return ENOENT;

	if (os_add_overflow(vp->v_iocount, 1, &vp->v_iocount)) {
		panic("v_iocount overflow");
	}

	return 0;
	return vget_internal(vp, vid, (VNODE_NODEAD | VNODE_WITHID | VNODE_DRAINO));
	return vget_internal(vp, vid, (VNODE_NODEAD | VNODE_WITHID));
	return vget_internal(vp, 0, 0);
	return vget_internal(vp, 0, VNODE_ALWAYS);
	int retval;
	return retval;
}

static inline void
vn_set_dead(vnode_t vp)
{
	vp->v_mount = NULL;
	vp->v_op = dead_vnodeop_p;
	vp->v_tag = VT_NON;
	vp->v_data = NULL;
	vp->v_type = VBAD;
	vp->v_lflag |= VL_DEAD;
	vfs_context_t ctx = vfs_context_current();      /* hoist outside loop */
	if (vp->v_iocount < 1) {
	}
	if ((vp->v_usecount > 0) || (vp->v_iocount > 1)) {
		return 0;
		vp->v_lflag &= ~VL_NEEDINACTIVE;
		vnode_unlock(vp);
	vp->v_lflag &= ~VL_NEEDINACTIVE;
		vnode_lock_convert(vp);
		vnode_reclaim_internal(vp, 1, 1, 0);
	return 0;
int
	return vnode_isinuse_locked(vp, refcnt, 0);
}

int
vnode_usecount(vnode_t vp)
{
	return vp->v_usecount;
int
vnode_iocount(vnode_t vp)
{
	return vp->v_iocount;
}
static int
	if (!locked) {
	}
	if ((vp->v_type != VREG) && ((vp->v_usecount - vp->v_kusecount) > refcnt)) {
	if (vp->v_type == VREG) {

	if (!locked) {
	}
	return retval;
errno_t
		vp->v_lflag &= ~VL_SUSPENDED;
	return 0;
		return EBUSY;
	/*
	 * xxx is this sufficient to check if a vnode_drain is
	return 0;

	if (vp->v_flag & VLOCKLOCAL) {
	}

static errno_t
		return ENOENT;
	while (vp->v_iocount > 1) {
	}
	return 0;
 * However, if the vnode is marked DIRTY, we want to pull it out much earlier
#define UNAGE_THRESHHOLD        25
#define UNAGE_DIRTYTHRESHHOLD    6
	int withvid = vflags & VNODE_WITHID;
		int sleepflg = 0;

		if (nodead && (vp->v_lflag & VL_DEAD) && ((vp->v_type == VBAD) || (vp->v_data == 0))) {
			return ENOENT;
		if ((vp->v_lflag & (VL_SUSPENDED | VL_DRAIN | VL_TERMINATE)) == 0) {
			return ENOENT;
			break;

		if (always != 0) {
		}
		 * If this vnode is getting drained, there are some cases where
		 * we can't block or, in case of tty vnodes, want to be
		 * interruptible.
		if (vp->v_lflag & VL_DRAIN) {
			/*
			 * In some situations, we want to get an iocount
			 * even if the vnode is draining to prevent deadlock,
			 * e.g. if we're in the filesystem, potentially holding
			 * resources that could prevent other iocounts from
			 * being released.
			 */
			if (beatdrain) {
				break;
			}
			/*
			 * Don't block if the vnode's mount point is unmounting as
			 * we may be the thread the unmount is itself waiting on
			 * Only callers who pass in vids (at this point, we've already
			 * handled nosusp and nodead) are expecting error returns
			 * from this function, so only we can only return errors for
			 * those. ENODEV is intended to inform callers that the call
			 * failed because an unmount is in progress.
			 */
			if (withvid && (vp->v_mount) && vfs_isunmount(vp->v_mount)) {
				return ENODEV;
			}

			if (vnode_istty(vp)) {
				sleepflg = PCATCH;
			}
			int error;

			error = msleep(&vp->v_lflag, &vp->v_lock,
			    (PVFS | sleepflg), "vnode getiocount", NULL);
			if (error) {
				return error;
			}
		} else {
		}
	if (withvid && vid != vp->v_id) {
		return ENOENT;
	if (++vp->v_references >= UNAGE_THRESHHOLD ||
	    (vp->v_flag & VISDIRTY && vp->v_references >= UNAGE_DIRTYTHRESHHOLD)) {
		vp->v_references = 0;
	return 0;
vnode_dropiocount(vnode_t vp)
	if (vp->v_iocount < 1) {
	}
	if ((vp->v_lflag & (VL_DRAIN | VL_SUSPENDED)) && (vp->v_iocount <= 1)) {
	}
	if (!locked) {
	}
	if (vnode_istty(vp) && (flags & REVOKEALL) && vp->v_usecount &&
	    (vp->v_iocount > 1)) {
		vnode_unlock(vp);
		VNOP_IOCTL(vp, TIOCREVOKE, (caddr_t)NULL, 0, vfs_context_kernel());
		vnode_lock(vp);
	}

	if (vp->v_type != VBAD) {
		vgone(vp, flags);               /* clean and reclaim the vnode */
	}
	if (vp->v_data) {
	}
	if (vp->v_numoutput) {
	}
	if (UBCINFOEXISTS(vp)) {
	}
	if (vp->v_parent) {
		panic("vnode_reclaim_internal: vparent not removed");
	}
	if (vp->v_name) {
		panic("vnode_reclaim_internal: vname not removed");
	}
		/*
		vnode_list_add(vp);
	}
	if (!locked) {
		vnode_unlock(vp);
static int
vnode_create_internal(uint32_t flavor, uint32_t size, void *data, vnode_t *vpp,
    int init_vnode)
	int existing_vnode;
	struct  uthread *ut;
	if (*vpp) {
		vp = *vpp;
		*vpp = NULLVP;
		existing_vnode = 1;
	} else {
		existing_vnode = 0;
	}
	if (init_vnode) {
		/* Do quick sanity check on the parameters. */
		if ((param == NULL) || (param->vnfs_vtype == VBAD)) {
			error = EINVAL;
			goto error_out;
		}
#if CONFIG_TRIGGERS
		if ((flavor == VNCREATE_TRIGGER) && (size == VNCREATE_TRIGGER_SIZE)) {
			tinfo = (struct vnode_trigger_param *)data;

			/* Validate trigger vnode input */
			if ((param->vnfs_vtype != VDIR) ||
			    (tinfo->vnt_resolve_func == NULL) ||
			    (tinfo->vnt_flags & ~VNT_VALID_MASK)) {
				error = EINVAL;
				goto error_out;
			}
			/* Fall through a normal create (params will be the same) */
			flavor = VNCREATE_FLAVOR;
			size = VCREATESIZE;
		if ((flavor != VNCREATE_FLAVOR) || (size != VCREATESIZE)) {
			error = EINVAL;
			goto error_out;
		}
	}
	if (!existing_vnode) {
		if ((error = new_vnode(&vp))) {
			return error;
		}
		if (!init_vnode) {
			/* Make it so that it can be released by a vnode_put) */
			vn_set_dead(vp);
			*vpp = vp;
			return 0;
		}
	} else {
		/*
		 * A vnode obtained by vnode_create_empty has been passed to
		 * vnode_initialize - Unset VL_DEAD set by vn_set_dead. After
		 * this point, it is set back on any error.
		 *
		 * N.B. vnode locking - We make the same assumptions as the
		 * "unsplit" vnode_create did - i.e. it is safe to update the
		 * vnode's fields without the vnode lock. This vnode has been
		 * out and about with the filesystem and hopefully nothing
		 * was done to the vnode between the vnode_create_empty and
		 * now when it has come in through vnode_initialize.
		 */
		vp->v_lflag &= ~VL_DEAD;
	}
	if (param->vnfs_markroot) {
	}
	if (param->vnfs_marksystem) {
	}
			vn_set_dead(vp);
			return error;
		}
		if (param->vnfs_mp->mnt_ioflags & MNT_IOFLAGS_IOSCHED_SUPPORTED) {
			memory_object_mark_io_tracking(vp->v_ubcinfo->ui_control);
#if CONFIG_FIRMLINKS
	vp->v_fmlink = NULLVP;
#endif
	vp->v_flag &= ~VFMLINKTARGET;

		/*
		 * mount if successful, which we would need to undo on a
			vn_set_dead(vp);
			return error;
		vp->v_tag = VT_DEVFS;           /* callers will reset if needed (bdevvp) */
		if ((nvp = checkalias(vp, param->vnfs_rdev))) {
			vp->v_data = NULL;
			if (maj < (u_int)nchrdev && cdevsw[maj].d_type == D_TTY) {
			}
		    sizeof(*fip), M_TEMP, M_WAITOK);
		bzero(fip, sizeof(struct fifoinfo));
		if (param->vnfs_mp->mnt_kern_flag & MNTK_LOCK_LOCAL) {
			vp->v_flag |= VLOCKLOCAL;
		}
			if ((vp->v_freelist.tqe_prev != (struct vnode **)0xdeadb)) {
			}
		} else {
		}
		if ((cnp->cn_flags & UNIONCREATED) == UNIONCREATED) {
		}
	ut = get_bsdthread_info(current_thread());

	if ((current_proc()->p_lflag & P_LRAGE_VNODES) ||
	    (ut->uu_flag & (UT_RAGE_VNODES | UT_KERN_RAGE_VNODES))) {
		/*
		 * process has indicated that it wants any
		 * vnodes created on its behalf to be rapidly
		 * aged to reduce the impact on the cached set
		 * of vnodes
		 *
		 * if UT_KERN_RAGE_VNODES is set, then the
		 * kernel internally wants vnodes to be rapidly
		 * aged, even if the process hasn't requested
		 * this
		 */
		vp->v_flag |= VRAGE;
	}

#if CONFIG_SECLUDED_MEMORY
	switch (secluded_for_filecache) {
	case 0:
		/*
		 * secluded_for_filecache == 0:
		 * + no file contents in secluded pool
		 */
		break;
	case 1:
		/*
		 * secluded_for_filecache == 1:
		 * + no files from /
		 * + files from /Applications/ are OK
		 * + files from /Applications/Camera are not OK
		 * + no files that are open for write
		 */
		if (vnode_vtype(vp) == VREG &&
		    vnode_mount(vp) != NULL &&
		    (!(vfs_flags(vnode_mount(vp)) & MNT_ROOTFS))) {
			/* not from root filesystem: eligible for secluded pages */
			memory_object_mark_eligible_for_secluded(
				ubc_getobject(vp, UBC_FLAGS_NONE),
				TRUE);
		}
		break;
	case 2:
		/*
		 * secluded_for_filecache == 2:
		 * + all read-only files OK, except:
		 *      + dyld_shared_cache_arm64*
		 *      + Camera
		 *	+ mediaserverd
		 */
		if (vnode_vtype(vp) == VREG) {
			memory_object_mark_eligible_for_secluded(
				ubc_getobject(vp, UBC_FLAGS_NONE),
				TRUE);
		}
		break;
	default:
		break;
	}
#endif /* CONFIG_SECLUDED_MEMORY */
	return 0;

error_out:
	if (existing_vnode) {
		vnode_put(vp);
	return error;
}

/* USAGE:
 * The following api creates a vnode and associates all the parameter specified in vnode_fsparam
 * structure and returns a vnode handle with a reference. device aliasing is handled here so checkalias
 * is obsoleted by this.
 */
int
vnode_create(uint32_t flavor, uint32_t size, void *data, vnode_t *vpp)
{
	*vpp = NULLVP;
	return vnode_create_internal(flavor, size, data, vpp, 1);
}

int
vnode_create_empty(vnode_t *vpp)
{
	*vpp = NULLVP;
	return vnode_create_internal(VNCREATE_FLAVOR, VCREATESIZE, NULL,
	           vpp, 0);
}

int
vnode_initialize(uint32_t flavor, uint32_t size, void *data, vnode_t *vpp)
{
	if (*vpp == NULLVP) {
		panic("NULL vnode passed to vnode_initialize");
	}
#if DEVELOPMENT || DEBUG
	/*
	 * We lock to check that vnode is fit for unlocked use in
	 * vnode_create_internal.
	 */
	vnode_lock_spin(*vpp);
	VNASSERT(((*vpp)->v_iocount == 1), *vpp,
	    ("vnode_initialize : iocount not 1, is %d", (*vpp)->v_iocount));
	VNASSERT(((*vpp)->v_usecount == 0), *vpp,
	    ("vnode_initialize : usecount not 0, is %d", (*vpp)->v_usecount));
	VNASSERT(((*vpp)->v_lflag & VL_DEAD), *vpp,
	    ("vnode_initialize : v_lflag does not have VL_DEAD, is 0x%x",
	    (*vpp)->v_lflag));
	VNASSERT(((*vpp)->v_data == NULL), *vpp,
	    ("vnode_initialize : v_data not NULL"));
	vnode_unlock(*vpp);
#endif
	return vnode_create_internal(flavor, size, data, vpp, 1);
	if (vp->v_lflag & VNAMED_FSHASH) {
	}
	if ((vp->v_freelist.tqe_prev != (struct vnode **)0xdeadb)) {
		panic("addfsref: vp on the free list\n");
	}
	return 0;
	if ((vp->v_lflag & VNAMED_FSHASH) == 0) {
	}
	return 0;
	mount_t mp;
	int count, actualcount, i;
	int cb_dropref = (flags & VFS_ITERATE_CB_DROPREF);
	int noskip_unmount = (flags & VFS_ITERATE_NOSKIP_UNMOUNT);
	} else { /* Head first by default */
	for (i = indx_start; i != indx_stop; i += indx_incr) {
		if (mp == (struct mount *)0) {
		}
		if ((mp->mnt_lflag & MNT_LDEAD) ||
		    (!noskip_unmount && (mp->mnt_lflag & MNT_LUNMOUNT))) {
		/*
		 * Drop the iterref here if the callback didn't do it.
		 * Note: If cb_dropref is set the mp may no longer exist.
		 */
		if (!cb_dropref) {
			mount_iterdrop(mp);
		}
	return ret;
	struct vfs_attr va;
	int             error;

	if ((error = vfs_getattr(mp, &va, ctx)) != 0) {
		KAUTH_DEBUG("STAT - filesystem returned error %d", error);
		return error;
	}
		if (error != 0) {
			return error;
		}
		mp->mnt_vfsstat.f_bsize = mp->mnt_devblocksize; /* default from the device block size */
		mp->mnt_vfsstat.f_iosize = 1024 * 1024;         /* 1MB sensible I/O size */
	if (VFSATTR_IS_SUPPORTED(&va, f_blocks)) {
	}
	if (VFSATTR_IS_SUPPORTED(&va, f_bfree)) {
	}
	if (VFSATTR_IS_SUPPORTED(&va, f_bavail)) {
	}
	if (VFSATTR_IS_SUPPORTED(&va, f_bused)) {
	}
	if (VFSATTR_IS_SUPPORTED(&va, f_files)) {
	}
	if (VFSATTR_IS_SUPPORTED(&va, f_ffree)) {
	}
	if (VFSATTR_IS_SUPPORTED(&va, f_fssubtype)) {
	}
	return 0;
		TAILQ_INSERT_TAIL(&mountlist, mp, mnt_list);
				if (mount_iterref(cur_mount, 1)) {
	return cur_mount;
mount_t
	if (!locked) {
	}
	TAILQ_FOREACH(mp, &mountlist, mnt_list)
	if (mp->mnt_vfsstat.f_fsid.val[0] == fsid->val[0] &&
	    mp->mnt_vfsstat.f_fsid.val[1] == fsid->val[1]) {
		retmp = mp;
		if (withref) {
			if (mount_iterref(retmp, 1)) {
				retmp = (mount_t)0;
		goto out;
	}
	if (!locked) {
	}
	return retmp;
vnode_lookupat(const char *path, int flags, vnode_t *vpp, vfs_context_t ctx,
    vnode_t start_dvp)
	if (ctx == NULL) {
		return EINVAL;
	if (flags & VNODE_LOOKUP_NOFOLLOW) {
	} else {
	}
	if (flags & VNODE_LOOKUP_NOCROSSMOUNT) {
	}

	if (flags & VNODE_LOOKUP_CROSSMOUNTNOWAIT) {
		ndflags |= CN_NBMOUNTLOOK;
	}
	    CAST_USER_ADDR_T(path), ctx);

	if (start_dvp && (path[0] != '/')) {
		nd.ni_dvp = start_dvp;
		nd.ni_cnd.cn_flags |= USEDVP;
	}

	if ((error = namei(&nd))) {
		return error;
	}

	nd.ni_cnd.cn_flags &= ~USEDVP;

	return 0;
}

errno_t
vnode_lookup(const char *path, int flags, vnode_t *vpp, vfs_context_t ctx)
{
	return vnode_lookupat(path, flags, vpp, ctx, NULLVP);
	if (ctx == NULL) {              /* XXX technically an error */
	if (fmode & O_NOFOLLOW) {
	}
	if (lflags & VNODE_LOOKUP_NOFOLLOW) {
	} else {
	}
	if (lflags & VNODE_LOOKUP_NOCROSSMOUNT) {
	}

	if (lflags & VNODE_LOOKUP_CROSSMOUNTNOWAIT) {
		ndflags |= CN_NBMOUNTLOOK;
	}

	    CAST_USER_ADDR_T(path), ctx);
	if ((error = vn_open(&nd, fmode, cmode))) {
	} else {
	}

	return error;

	return error;
}

errno_t
vnode_mtime(vnode_t vp, struct timespec *mtime, vfs_context_t ctx)
{
	struct vnode_attr       va;
	int                     error;

	VATTR_INIT(&va);
	VATTR_WANTED(&va, va_modify_time);
	error = vnode_getattr(vp, &va, ctx);
	if (!error) {
		*mtime = va.va_modify_time;
	}
	return error;
}

errno_t
vnode_flags(vnode_t vp, uint32_t *flags, vfs_context_t ctx)
{
	struct vnode_attr       va;
	int                     error;

	VATTR_INIT(&va);
	VATTR_WANTED(&va, va_flags);
	error = vnode_getattr(vp, &va, ctx);
	if (!error) {
		*flags = va.va_flags;
	}
	return error;
	struct vnode_attr       va;
	int                     error;
	if (!error) {
	}
	return error;
	struct vnode_attr       va;
	return vnode_setattr(vp, &va, ctx);
}

int
vnode_setdirty(vnode_t vp)
{
	vnode_lock_spin(vp);
	vp->v_flag |= VISDIRTY;
	vnode_unlock(vp);
	return 0;
}

int
vnode_cleardirty(vnode_t vp)
{
	vnode_lock_spin(vp);
	vp->v_flag &= ~VISDIRTY;
	vnode_unlock(vp);
	return 0;
}

int
vnode_isdirty(vnode_t vp)
{
	int dirty;

	vnode_lock_spin(vp);
	dirty = (vp->v_flag & VISDIRTY) ? 1 : 0;
	vnode_unlock(vp);

	return dirty;
		return VNOP_COMPOUND_OPEN(dvp, vpp, ndp, O_CREAT, fmode, statusp, vap, ctx);
 *
 *
 *
 *
	errno_t error, old_error;
	if (flags & VN_CREATE_NOINHERIT) {
	}
	if (flags & VN_CREATE_NOAUTH) {
	}

	switch (vap->va_type) {
		if (error) {
		}
			vp = NULLVP;
	/*
	 * For creation VNOPs, this is the equivalent of
	 * lookup_handle_found_vnode.
	 */
	if (kdebug_enable && *vpp) {
		kdebug_lookup(*vpp, cnp);
	}

	return error;
static kauth_scope_t    vnode_scope;
static int      vnode_authorize_callback(kauth_cred_t credential, void *idata, kauth_action_t action,
static int vnode_authorize_callback_int(kauth_action_t action, vfs_context_t ctx,
    vnode_t vp, vnode_t dvp, int *errorp);
	vnode_t         vp;
	vnode_t         dvp;
	vfs_context_t   ctx;
	int             flags;
	int             flags_valid;
#define _VAC_IS_OWNER           (1<<0)
#define _VAC_IN_GROUP           (1<<1)
#define _VAC_IS_DIR_OWNER       (1<<2)
#define _VAC_IN_DIR_GROUP       (1<<3)
#define _VAC_NO_VNODE_POINTERS  (1<<4)
#define VATTR_PREPARE_DEFAULTED_UID             0x1
#define VATTR_PREPARE_DEFAULTED_GID             0x2
#define VATTR_PREPARE_DEFAULTED_MODE            0x4
		    oacl,
		    &nacl,
		    vap->va_type == VDIR,
		    ctx)) != 0) {
			return error;

	}
		if (oacl) {
#if !CONFIG_MACF
#pragma unused(cnp)
#endif
	 * Normally, unlinking of directories is not supported.
	    !(vp->v_mount->mnt_kern_flag & MNTK_DIR_HARDLINKS)) {
		return EPERM; /* POSIX */
	if (!error) {
	}
	if (!error) {
	}
		if (error) {
			return error;
		}
	if ((fmode & O_DIRECTORY) && vp->v_type != VDIR) {
		return ENOTDIR;
		return EOPNOTSUPP;    /* Operation not supported on socket */
		return ELOOP;         /* O_NOFOLLOW was specified and the target is a symbolic link */
		return EISDIR;
			return ENOTDIR;
	/* If a file being opened is a shadow file containing
	 * namedstream data, ignore the macf checks because it
	 * is a kernel internal file and access should always
			return error;
	error = vnode_authorize(vp, NULL, action, ctx);
#if NAMEDSTREAMS
	if (error == EACCES) {
		/*
		 * Shadow files may exist on-disk with a different UID/GID
		 * than that of the current context.  Verify that this file
		 * is really a shadow file.  If it was created successfully
		 * then it should be authorized.
		 */
		if (vnode_isshadow(vp) && vnode_isnamedstream(vp)) {
			error = vnode_verifynamedstream(vp);
		}
	}
#endif

	return error;
#if !CONFIG_MACF
#pragma unused(vap)
#endif
		if (error) {
			return error;
		}
	if (error) {
		return error;
	}
	return vnode_authorize(dvp, NULL, KAUTH_VNODE_ADD_FILE, ctx);
int
vn_authorize_rename(struct vnode *fdvp, struct vnode *fvp, struct componentname *fcnp,
    struct vnode *tdvp, struct vnode *tvp, struct componentname *tcnp,
    vfs_context_t ctx, void *reserved)
{
	return vn_authorize_renamex(fdvp, fvp, fcnp, tdvp, tvp, tcnp, ctx, 0, reserved);
}

int
vn_authorize_renamex(struct vnode *fdvp, struct vnode *fvp, struct componentname *fcnp,
    struct vnode *tdvp, struct vnode *tvp, struct componentname *tcnp,
    vfs_context_t ctx, vfs_rename_flags_t flags, void *reserved)
{
	return vn_authorize_renamex_with_paths(fdvp, fvp, fcnp, NULL, tdvp, tvp, tcnp, NULL, ctx, flags, reserved);
}

int
vn_authorize_renamex_with_paths(struct vnode *fdvp, struct vnode *fvp, struct componentname *fcnp, const char *from_path,
    struct vnode *tdvp, struct vnode *tvp, struct componentname *tcnp, const char *to_path,
    vfs_context_t ctx, vfs_rename_flags_t flags, void *reserved)
	bool swap = flags & VFS_RENAME_SWAP;
	    (fcnp->cn_namelen == 1 && fcnp->cn_nameptr[0] == '.') ||
	    ((fcnp->cn_flags | tcnp->cn_flags) & ISDOTDOT))) {
		if (error) {
		}
	error = mac_vnode_check_rename(ctx, fdvp, fvp, fcnp, tdvp, tvp, tcnp);
	if (error) {
	}
	if (swap) {
		error = mac_vnode_check_rename(ctx, tdvp, tvp, tcnp, fdvp, fvp, fcnp);
		if (error) {
			goto out;
		}
	}
		if (!swap) {
			if (fvp->v_type == VDIR && tvp->v_type != VDIR) {
				error = ENOTDIR;
				goto out;
			} else if (fvp->v_type != VDIR && tvp->v_type == VDIR) {
				error = EISDIR;
				goto out;
			}
	} else if (swap) {
		/*
		 * Caller should have already checked this and returned
		 * ENOENT.  If we send back ENOENT here, caller will retry
		 * which isn't what we want so we send back EINVAL here
		 * instead.
		 */
		error = EINVAL;
		goto out;
	if (swap && fdvp->v_parent == tvp) {
		error = EINVAL;
		goto out;
	/***** </MiscChecks> *****/
	/***** <Kauth> *****/
	 * As part of the Kauth step, we call out to allow 3rd-party
	 * fileop notification of "about to rename".  This is needed
	 * in the event that 3rd-parties need to know that the DELETE
	 * authorization is actually part of a rename.  It's important
	 * that we guarantee that the DELETE call-out will always be
	 * made if the WILL_RENAME call-out is made.  Another fileop
	 * call-out will be performed once the operation is completed.
	 * We can ignore the result of kauth_authorize_fileop().
	 * N.B. We are passing the vnode and *both* paths to each
	 * call; kauth_authorize_fileop() extracts the "from" path
	 * when posting a KAUTH_FILEOP_WILL_RENAME notification.
	 * As such, we only post these notifications if all of the
	 * information we need is provided.

	if (swap) {
		kauth_action_t f = 0, t = 0;

		/*
		 * Directories changing parents need ...ADD_SUBDIR...  to
		 * permit changing ".."
		 */
		if (fdvp != tdvp) {
			if (vnode_isdir(fvp)) {
				f = KAUTH_VNODE_ADD_SUBDIRECTORY;
			}
			if (vnode_isdir(tvp)) {
				t = KAUTH_VNODE_ADD_SUBDIRECTORY;
			}
		}
		if (to_path != NULL) {
			kauth_authorize_fileop(vfs_context_ucred(ctx),
			    KAUTH_FILEOP_WILL_RENAME,
			    (uintptr_t)fvp,
			    (uintptr_t)to_path);
		}
		error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE | f, ctx);
		if (error) {
		}
		if (from_path != NULL) {
			kauth_authorize_fileop(vfs_context_ucred(ctx),
			    KAUTH_FILEOP_WILL_RENAME,
			    (uintptr_t)tvp,
			    (uintptr_t)from_path);
		}
		error = vnode_authorize(tvp, tdvp, KAUTH_VNODE_DELETE | t, ctx);
		if (error) {
		}
		f = vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE;
		t = vnode_isdir(tvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE;
		if (fdvp == tdvp) {
			error = vnode_authorize(fdvp, NULL, f | t, ctx);
		} else {
			error = vnode_authorize(fdvp, NULL, t, ctx);
			if (error) {
				goto out;
			}
			error = vnode_authorize(tdvp, NULL, f, ctx);
		}
		if (error) {
	} else {
		error = 0;
		if ((tvp != NULL) && vnode_isdir(tvp)) {
			if (tvp != fdvp) {
				moving = 1;
			}
		} else if (tdvp != fdvp) {
			moving = 1;
		}

		/*
		 * must have delete rights to remove the old name even in
		 * the simple case of fdvp == tdvp.
		 *
		 * If fvp is a directory, and we are changing it's parent,
		 * then we also need rights to rewrite its ".." entry as well.
		 */
		if (to_path != NULL) {
			kauth_authorize_fileop(vfs_context_ucred(ctx),
			    KAUTH_FILEOP_WILL_RENAME,
			    (uintptr_t)fvp,
			    (uintptr_t)to_path);
		}
		if (vnode_isdir(fvp)) {
			if ((error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE | KAUTH_VNODE_ADD_SUBDIRECTORY, ctx)) != 0) {
				goto out;
			}
		} else {
			if ((error = vnode_authorize(fvp, fdvp, KAUTH_VNODE_DELETE, ctx)) != 0) {
				goto out;
			}
		}
		if (moving) {
			/* moving into tdvp or tvp, must have rights to add */
			if ((error = vnode_authorize(((tvp != NULL) && vnode_isdir(tvp)) ? tvp : tdvp,
			    NULL,
			    vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE,
			    ctx)) != 0) {
				goto out;
			}
		} else {
			/* node staying in same directory, must be allowed to add new name */
			if ((error = vnode_authorize(fdvp, NULL,
			    vnode_isdir(fvp) ? KAUTH_VNODE_ADD_SUBDIRECTORY : KAUTH_VNODE_ADD_FILE, ctx)) != 0) {
				goto out;
			}
		}
		/* overwriting tvp */
		if ((tvp != NULL) && !vnode_isdir(tvp) &&
		    ((error = vnode_authorize(tvp, tdvp, KAUTH_VNODE_DELETE, ctx)) != 0)) {
		}
#if !CONFIG_MACF
#pragma unused(vap)
#endif
		panic("reserved not NULL in vn_authorize_mkdir()");
		if (error) {
		}
	if (error) {
	}
	/* authorize addition of a directory to the parent */
	if ((error = vnode_authorize(dvp, NULL, KAUTH_VNODE_ADD_SUBDIRECTORY, ctx)) != 0) {
		goto out;
	}

#if CONFIG_MACF
#else
#pragma unused(cnp)
#endif
	}

	}

	    vp, cnp);
	if (error) {
	}
/*
 * Authorizer for directory cloning. This does not use vnodes but instead
 * uses prefilled vnode attributes from the filesystem.
 *
 * The same function is called to set up the attributes required, perform the
 * authorization and cleanup (if required)
 */
int
vnode_attr_authorize_dir_clone(struct vnode_attr *vap, kauth_action_t action,
    struct vnode_attr *dvap, __unused vnode_t sdvp, mount_t mp,
    dir_clone_authorizer_op_t vattr_op, uint32_t flags, vfs_context_t ctx,
    __unused void *reserved)
{
	int error;
	int is_suser = vfs_context_issuser(ctx);

	if (vattr_op == OP_VATTR_SETUP) {
		VATTR_INIT(vap);

		/*
		 * When ACL inheritence is implemented, both vap->va_acl and
		 * dvap->va_acl will be required (even as superuser).
		 */
		VATTR_WANTED(vap, va_type);
		VATTR_WANTED(vap, va_mode);
		VATTR_WANTED(vap, va_flags);
		VATTR_WANTED(vap, va_uid);
		VATTR_WANTED(vap, va_gid);
		if (dvap) {
			VATTR_INIT(dvap);
			VATTR_WANTED(dvap, va_flags);
		}

		if (!is_suser) {
			/*
			 * If not superuser, we have to evaluate ACLs and
			 * need the target directory gid to set the initial
			 * gid of the new object.
			 */
			VATTR_WANTED(vap, va_acl);
			if (dvap) {
				VATTR_WANTED(dvap, va_gid);
			}
		} else if (dvap && (flags & VNODE_CLONEFILE_NOOWNERCOPY)) {
			VATTR_WANTED(dvap, va_gid);
		}
		return 0;
	} else if (vattr_op == OP_VATTR_CLEANUP) {
		return 0; /* Nothing to do for now */
	}

	/* dvap isn't used for authorization */
	error = vnode_attr_authorize(vap, NULL, mp, action, ctx);

	if (error) {
		return error;
	}

	/*
	 * vn_attribute_prepare should be able to accept attributes as well as
	 * vnodes but for now we do this inline.
	 */
	if (!is_suser || (flags & VNODE_CLONEFILE_NOOWNERCOPY)) {
		/*
		 * If the filesystem is mounted IGNORE_OWNERSHIP and an explicit
		 * owner is set, that owner takes ownership of all new files.
		 */
		if ((mp->mnt_flag & MNT_IGNORE_OWNERSHIP) &&
		    (mp->mnt_fsowner != KAUTH_UID_NONE)) {
			VATTR_SET(vap, va_uid, mp->mnt_fsowner);
		} else {
			/* default owner is current user */
			VATTR_SET(vap, va_uid,
			    kauth_cred_getuid(vfs_context_ucred(ctx)));
		}

		if ((mp->mnt_flag & MNT_IGNORE_OWNERSHIP) &&
		    (mp->mnt_fsgroup != KAUTH_GID_NONE)) {
			VATTR_SET(vap, va_gid, mp->mnt_fsgroup);
		} else {
			/*
			 * default group comes from parent object,
			 * fallback to current user
			 */
			if (VATTR_IS_SUPPORTED(dvap, va_gid)) {
				VATTR_SET(vap, va_gid, dvap->va_gid);
			} else {
				VATTR_SET(vap, va_gid,
				    kauth_cred_getgid(vfs_context_ucred(ctx)));
			}
		}
	}

	/* Inherit SF_RESTRICTED bit from destination directory only */
	if (VATTR_IS_ACTIVE(vap, va_flags)) {
		VATTR_SET(vap, va_flags,
		    ((vap->va_flags & ~(UF_DATAVAULT | SF_RESTRICTED)))); /* Turn off from source */
		if (VATTR_IS_ACTIVE(dvap, va_flags)) {
			VATTR_SET(vap, va_flags,
			    vap->va_flags | (dvap->va_flags & (UF_DATAVAULT | SF_RESTRICTED)));
		}
	} else if (VATTR_IS_ACTIVE(dvap, va_flags)) {
		VATTR_SET(vap, va_flags, (dvap->va_flags & (UF_DATAVAULT | SF_RESTRICTED)));
	}

	return 0;
}


	int     error, result;
	if (vp->v_type == VBAD) {
		return 0;
	}

	    (uintptr_t)ctx, (uintptr_t)vp, (uintptr_t)dvp, (uintptr_t)&error);
	if (result == EPERM) {          /* traditional behaviour */
	}
	if ((result != 0) && (error != 0)) {
		return error;
	}
	return result;
	int     mask;
	if (append) {
	}
	if ((vap->va_flags & mask) != 0) {
		return EPERM;
	}
	return 0;

	return result;
	int     error;
	int     result;
		if (error == ENOENT) {
		}
	if (!error) {
	}
	return error;
	return result;
	int     error;
	return error;
	return result;
	int     error;
	return error;
# define _SETWHERE(c)   where = c;


	if (action & VREAD) {
	}
	if (action & VWRITE) {
	}
	if (action & VEXEC) {
	}
	if (action & VREAD) {
	}
	if (action & VWRITE) {
	}
	if (action & VEXEC) {
	}
	if (action & VREAD) {
	}
	if (action & VWRITE) {
	}
	if (action & VEXEC) {
	}
		if (!owner_ok) {
		}
	ismember = 0;   /* Default to allow, if the target has no group owner */
		if (!group_ok) {
		}
		if (!group_ok) {
		}
	if (!world_ok) {
	}
	return error;
 * The precedence of factors for authorizing or denying delete for a credential
 *
 * 1) Explicit ACE on the node. (allow or deny DELETE)
 * 2) Explicit ACE on the directory (allow or deny DELETE_CHILD).
 *
 *    If there are conflicting ACEs on the node and the directory, the node
 *    ACE wins.
 * 3) Sticky bit on the directory.
 *    Deletion is not permitted if the directory is sticky and the caller is
 *    not owner of the node or directory. The sticky bit rules are like a deny
 *    delete ACE except lower in priority than ACL's either allowing or denying
 *    delete.
 *
 * 4) POSIX permisions on the directory.
 * on directories. This enables us to skip directory ACL and POSIX checks
 * as we already have the result from those checks. However, we always check the
 * node ACL and, if the directory has the sticky bit set, we always check its
 * ACL (even for a directory with an authorized delete child). Furthermore,
 * caching the delete child authorization is independent of the sticky bit
 * being set as it is only applicable in determining whether the node can be
 * deleted or not.
static int
	struct vnode_attr       *vap = vcp->vap;
	struct vnode_attr       *dvap = vcp->dvap;
	kauth_cred_t            cred = vcp->ctx->vc_ucred;
	struct kauth_acl_eval   eval;
	int                     error, ismember;
	/* Check the ACL on the node first */
	if (VATTR_IS_NOT(vap, va_acl, NULL)) {
		eval.ae_requested = KAUTH_VNODE_DELETE;
		eval.ae_acl = &vap->va_acl->acl_ace[0];
		eval.ae_count = vap->va_acl->acl_entrycount;
		if (vauth_file_owner(vcp)) {
		}
		if ((error = vauth_file_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT) {
			return error;
		}
		if (error == ENOENT) {
		} else if (ismember) {
		}
		if ((error = kauth_acl_evaluate(cred, &eval)) != 0) {
			return error;

		switch (eval.ae_result) {
			KAUTH_DEBUG("%p    DENIED - denied by ACL", vcp->vp);
			return EACCES;
		case KAUTH_RESULT_ALLOW:
			KAUTH_DEBUG("%p    ALLOWED - granted by ACL", vcp->vp);
			return 0;
			/* Defer to directory */
			KAUTH_DEBUG("%p    DEFERRED - by file ACL", vcp->vp);
	/*
	 * Without a sticky bit, a previously authorized delete child is
	 * sufficient to authorize this delete.
	 *
	 * If the sticky bit is set, a directory ACL which allows delete child
	 * overrides a (potential) sticky bit deny. The authorized delete child
	 * cannot tell us if it was authorized because of an explicit delete
	 * child allow ACE or because of POSIX permisions so we have to check
	 * the directory ACL everytime if the directory has a sticky bit.
	 */
	if (!(dvap->va_mode & S_ISTXT) && cached_delete_child) {
		KAUTH_DEBUG("%p    ALLOWED - granted by directory ACL or POSIX permissions and no sticky bit on directory", vcp->vp);
		return 0;
	}

	/* check the ACL on the directory */
	if (VATTR_IS_NOT(dvap, va_acl, NULL)) {
		eval.ae_requested = KAUTH_VNODE_DELETE_CHILD;
		eval.ae_acl = &dvap->va_acl->acl_ace[0];
		eval.ae_count = dvap->va_acl->acl_entrycount;
		if (vauth_dir_owner(vcp)) {
		}
		if ((error = vauth_dir_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT) {
			return error;
		}
		if (error == ENOENT) {
		} else if (ismember) {
		}
		/*
		 * If there is no entry, we are going to defer to other
		 * authorization mechanisms.
		 */
		error = kauth_acl_evaluate(cred, &eval);

		if (error != 0) {
			return error;
		switch (eval.ae_result) {
			KAUTH_DEBUG("%p    DENIED - denied by directory ACL", vcp->vp);
			return EACCES;
			KAUTH_DEBUG("%p    ALLOWED - granted by directory ACL", vcp->vp);
			if (!cached_delete_child && vcp->dvp) {
				vnode_cache_authorized_action(vcp->dvp,
				    vcp->ctx, KAUTH_VNODE_DELETE_CHILD);
			}
			return 0;
			/* Deferred by directory ACL */
			KAUTH_DEBUG("%p    DEFERRED - directory ACL", vcp->vp);
	/*
	 * From this point, we can't explicitly allow and if we reach the end
	 * of the function without a denial, then the delete is authorized.
	 */
	if (!cached_delete_child) {
		if (vnode_authorize_posix(vcp, VWRITE, 1 /* on_dir */) != 0) {
			KAUTH_DEBUG("%p    DENIED - denied by posix permisssions", vcp->vp);
			return EACCES;
		}
		/*
		 * Cache the authorized action on the vnode if allowed by the
		 * directory ACL or POSIX permissions. It is correct to cache
		 * this action even if sticky bit would deny deleting the node.
		 */
		if (vcp->dvp) {
			vnode_cache_authorized_action(vcp->dvp, vcp->ctx,
			    KAUTH_VNODE_DELETE_CHILD);
		}
		return EACCES;
	return 0;

	struct vnode_attr       *vap = vcp->vap;
	kauth_cred_t            cred = vcp->ctx->vc_ucred;
	struct kauth_acl_eval   eval;
	int                     error, ismember;
	mode_t                  posix_action;
	if (vauth_file_owner(vcp)) {
	}
	    (acl_rights & KAUTH_VNODE_WRITE_SECURITY)) {
	}

		return 0;
		if (vauth_file_owner(vcp)) {
		}
		if ((error = vauth_file_ingroup(vcp, &ismember, ENOENT)) != 0 && error != ENOENT) {
			return error;
		}
		if (error == ENOENT) {
		} else if (ismember) {
		}

			return error;

		switch (eval.ae_result) {
			return EACCES;         /* deny, deny, counter-allege */
			return 0;
	if (vauth_file_owner(vcp)) {
	}

		return 0;
	}

		return EACCES;
		return EACCES;
	if (eval.ae_residual & KAUTH_VNODE_DELETE) {
	}
	    KAUTH_VNODE_LIST_DIRECTORY |
	    KAUTH_VNODE_READ_EXTATTRIBUTES)) {
	}
	    KAUTH_VNODE_ADD_FILE |
	    KAUTH_VNODE_ADD_SUBDIRECTORY |
	    KAUTH_VNODE_DELETE_CHILD |
	    KAUTH_VNODE_WRITE_ATTRIBUTES |
	    KAUTH_VNODE_WRITE_EXTATTRIBUTES)) {
	}
	    KAUTH_VNODE_SEARCH)) {
	}

		return vnode_authorize_posix(vcp, posix_action, 0 /* !on_dir */);
	return 0;
vnode_authorize_checkimmutable(mount_t mp, struct vnode_attr *vap, int rights, int ignore)
	switch (vap->va_type) {
				KAUTH_DEBUG("%p    DENIED - filesystem disallowed extended attributes", vap);
		/*
		 * check for file immutability. first, check if the requested rights are
		if (vap->va_type == VDIR) {
			if ((rights & (KAUTH_VNODE_ADD_FILE | KAUTH_VNODE_ADD_SUBDIRECTORY | KAUTH_VNODE_WRITE_EXTATTRIBUTES)) == rights) {
			}
			if ((rights & (KAUTH_VNODE_APPEND_DATA | KAUTH_VNODE_WRITE_EXTATTRIBUTES)) == rights) {
			}
			KAUTH_DEBUG("%p    DENIED - file is immutable", vap);
	return error;
	int     error;
	switch (vp->v_type) {
		return 0;
	if ((action & KAUTH_VNODE_ACCESS) && !vfs_authopaqueaccess(vp->v_mount)) {
		return 0;
	}
		return 1;

			return 1;
	return 1;
vnode_authorize_callback(__unused kauth_cred_t cred, __unused void *idata,
    kauth_action_t action, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2,
    uintptr_t arg3)
	vfs_context_t   ctx;
	vnode_t         cvp = NULLVP;
	vnode_t         vp, dvp;
	int             result = KAUTH_RESULT_DENY;
	int             parent_iocount = 0;
	int             parent_action; /* In case we need to use namedstream's data fork for cached rights*/
	 * this point which rights to look at based on the
	if (dvp && vp) {
		goto defer;
	}
		cvp = dvp;
		/*
		 * on the parent means that we don't recompute for the named stream and that if
		 * as if there were no cached rights (passing the named stream vnode and desired rights to
		 * On an opaquely authorized volume, we don't know the relationship between the
		 * stream. On a VNOP_SETATTR() for the parent that may invalidate cached authorization, we
		} else {
			cvp = vp;
		}
	}

	if (vnode_cache_is_authorized(cvp, ctx, parent_iocount ? parent_action : action) == TRUE) {
		result = KAUTH_RESULT_ALLOW;
		goto out;
	}
defer:
	result = vnode_authorize_callback_int(action, ctx, vp, dvp, (int *)arg3);

	if (result == KAUTH_RESULT_ALLOW && cvp != NULLVP) {
		KAUTH_DEBUG("%p - caching action = %x", cvp, action);
		vnode_cache_authorized_action(cvp, ctx, action);
	}

out:
	if (parent_iocount) {
		vnode_put(cvp);
	}

	return result;
}

static int
vnode_attr_authorize_internal(vauth_ctx vcp, mount_t mp,
    kauth_ace_rights_t rights, int is_suser, boolean_t *found_deny,
    int noimmutable, int parent_authorized_for_delete_child)
{
	int result;

	/*
	 * Check for immutability.
	 *
	 * In the deletion case, parent directory immutability vetoes specific
	 * file rights.
	 */
	if ((result = vnode_authorize_checkimmutable(mp, vcp->vap, rights,
	    noimmutable)) != 0) {
		goto out;
	}
	if ((rights & KAUTH_VNODE_DELETE) &&
	    !parent_authorized_for_delete_child) {
		result = vnode_authorize_checkimmutable(mp, vcp->dvap,
		    KAUTH_VNODE_DELETE_CHILD, 0);
		if (result) {
			goto out;
	/*
	 * Clear rights that have been authorized by reaching this point, bail if nothing left to
	 * check.
	 */
	rights &= ~(KAUTH_VNODE_LINKTARGET | KAUTH_VNODE_CHECKIMMUTABLE);
	if (rights == 0) {
	/*
	 * If we're not the superuser, authorize based on file properties;
	 * note that even if parent_authorized_for_delete_child is TRUE, we
	 * need to check on the node itself.
	 */
	if (!is_suser) {
		/* process delete rights */
		if ((rights & KAUTH_VNODE_DELETE) &&
		    ((result = vnode_authorize_delete(vcp, parent_authorized_for_delete_child)) != 0)) {
			goto out;
		}
		/* process remaining rights */
		if ((rights & ~KAUTH_VNODE_DELETE) &&
		    (result = vnode_authorize_simple(vcp, rights, rights & KAUTH_VNODE_DELETE, found_deny)) != 0) {
			goto out;
		}
	} else {
		/*
		 * Execute is only granted to root if one of the x bits is set.  This check only
		 * makes sense if the posix mode bits are actually supported.
		 */
		if ((rights & KAUTH_VNODE_EXECUTE) &&
		    (vcp->vap->va_type == VREG) &&
		    VATTR_IS_SUPPORTED(vcp->vap, va_mode) &&
		    !(vcp->vap->va_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
			result = EPERM;
			KAUTH_DEBUG("%p    DENIED - root execute requires at least one x bit in 0x%x", vcp, vcp->vap->va_mode);
			goto out;
		}
		/* Assume that there were DENYs so we don't wrongly cache KAUTH_VNODE_SEARCHBYANYONE */
		*found_deny = TRUE;

		KAUTH_DEBUG("%p    ALLOWED - caller is superuser", vcp);
	}
out:
vnode_authorize_callback_int(kauth_action_t action, vfs_context_t ctx,
    vnode_t vp, vnode_t dvp, int *errorp)
	vauth_ctx               vcp;
	kauth_cred_t            cred;
	kauth_ace_rights_t      rights;
	struct vnode_attr       va, dva;
	int                     result;
	int                     noimmutable;
	boolean_t               parent_authorized_for_delete_child = FALSE;
	boolean_t               found_deny = FALSE;
	boolean_t               parent_ref = FALSE;
	boolean_t               is_suser = FALSE;
	vcp->ctx = ctx;
	vcp->vp = vp;
	vcp->dvp = dvp;
	if ((ctx == NULL) || (vp == NULL) || (cred == NULL)) {
	}
	    (action & KAUTH_VNODE_ACCESS)               ? "access" : "auth",
	    (action & KAUTH_VNODE_READ_DATA)            ? vnode_isdir(vp) ? " LIST_DIRECTORY" : " READ_DATA" : "",
	    (action & KAUTH_VNODE_WRITE_DATA)           ? vnode_isdir(vp) ? " ADD_FILE" : " WRITE_DATA" : "",
	    (action & KAUTH_VNODE_EXECUTE)              ? vnode_isdir(vp) ? " SEARCH" : " EXECUTE" : "",
	    (action & KAUTH_VNODE_DELETE)               ? " DELETE" : "",
	    (action & KAUTH_VNODE_APPEND_DATA)          ? vnode_isdir(vp) ? " ADD_SUBDIRECTORY" : " APPEND_DATA" : "",
	    (action & KAUTH_VNODE_DELETE_CHILD)         ? " DELETE_CHILD" : "",
	    (action & KAUTH_VNODE_READ_ATTRIBUTES)      ? " READ_ATTRIBUTES" : "",
	    (action & KAUTH_VNODE_WRITE_ATTRIBUTES)     ? " WRITE_ATTRIBUTES" : "",
	    (action & KAUTH_VNODE_READ_EXTATTRIBUTES)   ? " READ_EXTATTRIBUTES" : "",
	    (action & KAUTH_VNODE_WRITE_EXTATTRIBUTES)  ? " WRITE_EXTATTRIBUTES" : "",
	    (action & KAUTH_VNODE_READ_SECURITY)        ? " READ_SECURITY" : "",
	    (action & KAUTH_VNODE_WRITE_SECURITY)       ? " WRITE_SECURITY" : "",
	    (action & KAUTH_VNODE_CHANGE_OWNER)         ? " CHANGE_OWNER" : "",
	    (action & KAUTH_VNODE_NOIMMUTABLE)          ? " (noimmutable)" : "",

		if (dvp == NULL) {
		}
		if (vnode_cache_is_authorized(dvp, ctx, KAUTH_VNODE_DELETE_CHILD) == TRUE) {
			parent_authorized_for_delete_child = TRUE;
		}
		vcp->dvp = NULLVP;
		vcp->dvap = NULL;

	    ((vp->v_type == VREG) || (vp->v_type == VDIR) ||
	    (vp->v_type == VLNK) || (vp->v_type == VCPLX) ||
	    (rights & KAUTH_VNODE_DELETE) || (rights & KAUTH_VNODE_DELETE_CHILD))) {
	if ((vp->v_mount->mnt_kern_flag & MNTK_AUTH_OPAQUE) && vnode_authorize_opaque(vp, &result, action, ctx)) {
	 * If the vnode is a namedstream (extended attribute) data vnode (eg.
	 * a resource fork), *_DATA becomes *_EXTATTRIBUTES.

		/*
		 * Point 'vp' to the namedstream's parent for ACL checking
		 */
		if ((vp->v_parent != NULL) &&
		    (vget_internal(vp->v_parent, 0, VNODE_NODEAD | VNODE_DRAINO) == 0)) {
			parent_ref = TRUE;
			vcp->vp = vp = vp->v_parent;
		}
	}

	if (vfs_context_issuser(ctx)) {
		/*
		 * if we're not asking for execute permissions or modifications,
		 * then we're done, this action is authorized.
		 */
		if (!(rights & (KAUTH_VNODE_EXECUTE | KAUTH_VNODE_WRITE_RIGHTS))) {
			goto success;
		}

		is_suser = TRUE;
	 * Get vnode attributes and extended security information for the vnode
	 * and directory if required.
	 *
	 * If we're root we only want mode bits and flags for checking
	 * execute and immutability.
	VATTR_WANTED(&va, va_mode);
	VATTR_WANTED(&va, va_flags);
	if (!is_suser) {
	if ((result = vnode_getattr(vp, &va, ctx)) != 0) {
		KAUTH_DEBUG("%p    ERROR - failed to get vnode attributes - %d", vp, result);
	}
	VATTR_WANTED(&va, va_type);
	VATTR_RETURN(&va, va_type, vnode_vtype(vp));
	if (vcp->dvp) {
		VATTR_WANTED(&dva, va_mode);
		VATTR_WANTED(&dva, va_flags);
		if (!is_suser) {
			VATTR_WANTED(&dva, va_uid);
			VATTR_WANTED(&dva, va_gid);
			VATTR_WANTED(&dva, va_acl);
		}
		if ((result = vnode_getattr(vcp->dvp, &dva, ctx)) != 0) {
			KAUTH_DEBUG("%p    ERROR - failed to get directory vnode attributes - %d", vp, result);
		VATTR_WANTED(&dva, va_type);
		VATTR_RETURN(&dva, va_type, vnode_vtype(vcp->dvp));

	result = vnode_attr_authorize_internal(vcp, vp->v_mount, rights, is_suser,
	    &found_deny, noimmutable, parent_authorized_for_delete_child);
	if (VATTR_IS_SUPPORTED(&va, va_acl) && (va.va_acl != NULL)) {
	}
	if (VATTR_IS_SUPPORTED(&dva, va_acl) && (dva.va_acl != NULL)) {
	}
		if (parent_ref) {
		}
		return KAUTH_RESULT_DENY;
		/*
		 * deny execute, we can synthesize a global right that allows anyone to
		 *
		 * Note that we can correctly cache KAUTH_VNODE_SEARCHBYANYONE
		 * only if we actually check ACLs which we don't for root. As
		 * a workaround, the lookup fast path checks for root.
		if (!VATTR_IS_SUPPORTED(&va, va_mode) ||
		    (S_IXUSR | S_IXGRP | S_IXOTH))) {
			vnode_cache_authorized_action(vp, ctx, KAUTH_VNODE_SEARCHBYANYONE);
success:
	if (parent_ref) {
	}

	return KAUTH_RESULT_ALLOW;
}

int
vnode_attr_authorize_init(struct vnode_attr *vap, struct vnode_attr *dvap,
    kauth_action_t action, vfs_context_t ctx)
{
	VATTR_INIT(vap);
	VATTR_WANTED(vap, va_type);
	VATTR_WANTED(vap, va_mode);
	VATTR_WANTED(vap, va_flags);
	if (dvap) {
		VATTR_INIT(dvap);
		if (action & KAUTH_VNODE_DELETE) {
			VATTR_WANTED(dvap, va_type);
			VATTR_WANTED(dvap, va_mode);
			VATTR_WANTED(dvap, va_flags);
		}
	} else if (action & KAUTH_VNODE_DELETE) {
		return EINVAL;
	}

	if (!vfs_context_issuser(ctx)) {
		VATTR_WANTED(vap, va_uid);
		VATTR_WANTED(vap, va_gid);
		VATTR_WANTED(vap, va_acl);
		if (dvap && (action & KAUTH_VNODE_DELETE)) {
			VATTR_WANTED(dvap, va_uid);
			VATTR_WANTED(dvap, va_gid);
			VATTR_WANTED(dvap, va_acl);
		}
	}

	return 0;
}

int
vnode_attr_authorize(struct vnode_attr *vap, struct vnode_attr *dvap, mount_t mp,
    kauth_action_t action, vfs_context_t ctx)
{
	struct _vnode_authorize_context auth_context;
	vauth_ctx vcp;
	kauth_ace_rights_t rights;
	int noimmutable;
	boolean_t found_deny;
	boolean_t is_suser = FALSE;
	int result = 0;

	vcp = &auth_context;
	vcp->ctx = ctx;
	vcp->vp = NULLVP;
	vcp->vap = vap;
	vcp->dvp = NULLVP;
	vcp->dvap = dvap;
	vcp->flags = vcp->flags_valid = 0;

	noimmutable = (action & KAUTH_VNODE_NOIMMUTABLE) ? 1 : 0;
	rights = action & ~(KAUTH_VNODE_ACCESS | KAUTH_VNODE_NOIMMUTABLE);

	/*
	 * Check for read-only filesystems.
	 */
	if ((rights & KAUTH_VNODE_WRITE_RIGHTS) &&
	    mp && (mp->mnt_flag & MNT_RDONLY) &&
	    ((vap->va_type == VREG) || (vap->va_type == VDIR) ||
	    (vap->va_type == VLNK) || (rights & KAUTH_VNODE_DELETE) ||
	    (rights & KAUTH_VNODE_DELETE_CHILD))) {
		result = EROFS;
		goto out;
	}

	/*
	 * Check for noexec filesystems.
	 */
	if ((rights & KAUTH_VNODE_EXECUTE) &&
	    (vap->va_type == VREG) && mp && (mp->mnt_flag & MNT_NOEXEC)) {
		result = EACCES;
		goto out;
	}

	if (vfs_context_issuser(ctx)) {
		/*
		 * if we're not asking for execute permissions or modifications,
		 * then we're done, this action is authorized.
		 */
		if (!(rights & (KAUTH_VNODE_EXECUTE | KAUTH_VNODE_WRITE_RIGHTS))) {
			goto out;
		}
		is_suser = TRUE;
	} else {
		if (!VATTR_IS_SUPPORTED(vap, va_uid) ||
		    !VATTR_IS_SUPPORTED(vap, va_gid) ||
		    (mp && vfs_extendedsecurity(mp) && !VATTR_IS_SUPPORTED(vap, va_acl))) {
			panic("vnode attrs not complete for vnode_attr_authorize\n");
		}
	}

	result = vnode_attr_authorize_internal(vcp, mp, rights, is_suser,
	    &found_deny, noimmutable, FALSE);

	if (result == EPERM) {
		result = EACCES;
	}
out:
	return result;

int
	int             error;
	int             has_priv_suser, ismember, defaulted_owner, defaulted_group, defaulted_mode;
	uint32_t        inherit_flags;
	kauth_cred_t    cred;
	guid_t          changer;
	mount_t         dmp;
	struct vnode_attr dva;
	inherit_flags = 0;


	/*
	 * We need the dvp's va_flags and *may* need the gid of the directory,
	 * we ask for both here.
	 */
	VATTR_INIT(&dva);
	VATTR_WANTED(&dva, va_gid);
	VATTR_WANTED(&dva, va_flags);
	if ((error = vnode_getattr(dvp, &dva, ctx)) != 0) {
		goto out;
	}

	if (!VATTR_IS_ACTIVE(vap, va_flags)) {
	}

	/* Determine if SF_RESTRICTED should be inherited from the parent
	 * directory. */
	if (VATTR_IS_SUPPORTED(&dva, va_flags)) {
		inherit_flags = dva.va_flags & (UF_DATAVAULT | SF_RESTRICTED);
	}


	if (vfs_authopaque(dvp->v_mount)) {
		goto out;
	}
		vap->va_flags &= ~SF_SYNTHETIC;
out:
	if (inherit_flags) {
		/* Apply SF_RESTRICTED to the file if its parent directory was
		 * restricted.  This is done at the end so that root is not
		 * required if this flag is only set due to inheritance. */
		VATTR_SET(vap, va_flags, (vap->va_flags | inherit_flags));
	}
	return error;
	kauth_action_t  required_action;
	int             error, has_priv_suser, ismember, chowner, chgroup, clear_suid, clear_sgid;
	guid_t          changer;
	gid_t           group;
	uid_t           owner;
	mode_t          newmode;
	kauth_cred_t    cred;
	uint32_t        fdelta;
	if (vfs_authopaque(vp->v_mount)) {
	}


	    VATTR_IS_ACTIVE(vap, va_backup_time) ||
	    VATTR_IS_ACTIVE(vap, va_addedtime)) {
#if 0   /* enable this when we support UUIDs as official owners */

		/* size is only meaningful on regular files, don't permit otherwise */
		if (!vnode_isreg(vp)) {
			KAUTH_DEBUG("ATTR - ERROR: size change requested on non-file");
			error = vnode_isdir(vp) ? EISDIR : EINVAL;
		KAUTH_DEBUG("ATTR - size change, requiring WRITE_DATA");
		required_action |= KAUTH_VNODE_WRITE_DATA;
	    VATTR_IS_ACTIVE(vap, va_backup_time) ||
	    VATTR_IS_ACTIVE(vap, va_addedtime)) {
			required_action |= KAUTH_VNODE_CHECKIMMUTABLE;  /* always required */
			required_action |= KAUTH_VNODE_CHECKIMMUTABLE;  /* always required */

	 *
		vap->va_flags &= ~SF_SYNTHETIC;
		ova.va_flags &= ~SF_SYNTHETIC;
			if (!has_priv_suser && (kauth_cred_getuid(cred) != vap->va_uid)) {
				KAUTH_DEBUG("  DENIED - non-superuser cannot change ownershipt to a third party");
				error = EPERM;
				goto out;
			}
			chowner = 1;

			if (!has_priv_suser) {
				if ((error = kauth_cred_ismember_gid(cred, vap->va_gid, &ismember)) != 0) {
					KAUTH_DEBUG("  ERROR - got %d checking for membership in %d", error, vap->va_gid);
					goto out;
				}
				if (!ismember) {
					KAUTH_DEBUG("  DENIED - group change from %d to %d but not a member of target group",
					    ova.va_gid, vap->va_gid);
					error = EPERM;
					goto out;
				}
			chgroup = 1;
			if (kauth_guid_equal(&vap->va_uuuid, &ova.va_uuuid)) {
			}

				    kauth_guid_equal(&vap->va_uuuid, &uid_guid)) {
					goto no_uuuid_change;
				}

			if (kauth_guid_equal(&vap->va_guuid, &ova.va_guuid)) {
			}
				    kauth_guid_equal(&vap->va_guuid, &gid_guid)) {
					goto no_guuid_change;
				}
			if (kauth_guid_equal(&vap->va_guuid, &kauth_null_guid)) {
			} else if ((error = kauth_cred_ismember_guid(cred, &vap->va_guuid, &ismember)) != 0) {
				required_action |= KAUTH_VNODE_WRITE_SECURITY;
		}

		/*
		 * clear set-uid and set-gid bits. POSIX only requires this for
		 * non-privileged processes but we do it even for root.
		 */
		if (VATTR_IS_ACTIVE(vap, va_mode)) {
			newmode = vap->va_mode;
		} else if (VATTR_IS_SUPPORTED(&ova, va_mode)) {
			newmode = ova.va_mode;
		} else {
			KAUTH_DEBUG("CHOWN - trying to change owner but cannot get mode from filesystem to mask setugid bits");
			newmode = 0;
		}

		/* chown always clears setuid/gid bits. An exception is made for
		 * setattrlist which can set both at the same time: <uid, gid, mode> on a file:
		 * setattrlist is allowed to set the new mode on the file and change (chown)
		 * uid/gid.
		 */
		if (newmode & (S_ISUID | S_ISGID)) {
			if (!VATTR_IS_ACTIVE(vap, va_mode)) {
				KAUTH_DEBUG("CHOWN - masking setugid bits from mode %o to %o",
				    newmode, newmode & ~(S_ISUID | S_ISGID));
				newmode &= ~(S_ISUID | S_ISGID);
			VATTR_SET(vap, va_mode, newmode);
				    sizeof(struct kauth_ace) * vap->va_acl->acl_entrycount)) {
	if (VATTR_IS_ACTIVE(vap, va_encoding)) {
	}

	if (VATTR_IS_SUPPORTED(&ova, va_acl) && (ova.va_acl != NULL)) {
	}
	if (error == 0) {
	}
	return error;
	return VNODE_RETURNED;
vfs_setcompoundopen(mount_t mp)
	mp->mnt_compound_ops |= COMPOUND_VNOP_OPEN;
vnode_setswapmount(vnode_t vp)
	mount_lock(vp->v_mount);
	vp->v_mount->mnt_kern_flag |= MNTK_SWAP_MOUNT;
	mount_unlock(vp->v_mount);
}


int64_t
vnode_getswappin_avail(vnode_t vp)
{
	int64_t max_swappin_avail = 0;

	mount_lock(vp->v_mount);
	if (vp->v_mount->mnt_ioflags & MNT_IOFLAGS_SWAPPIN_SUPPORTED) {
		max_swappin_avail = vp->v_mount->mnt_max_swappin_available;
	}
	mount_unlock(vp->v_mount);

	return max_swappin_avail;

	while ((vp->v_flag & VISUNION) == VISUNION) {
	}
	if (!locked) {
	}
	if ((vp->v_flag & VISUNION) == VISUNION) {
	if (!locked) {
	}
vnode_materialize_dataless_file(vnode_t vp, uint64_t op_type)
	int error;

	/* Swap files are special; ignore them */
	if (vnode_isswap(vp)) {
		return 0;
	}

	error = resolve_nspace_item(vp,
	    op_type | NAMESPACE_HANDLER_NSPACE_EVENT);

	/*
	 * The file resolver owns the logic about what error to return
	 * to the caller.  We only need to handle a couple of special
	 * cases here:
	 */
	if (error == EJUSTRETURN) {
		/*
		 * The requesting process is allowed to interact with
		 * dataless objects.  Make a couple of sanity-checks
		 * here to ensure the action makes sense.
		 */
		switch (op_type) {
		case NAMESPACE_HANDLER_WRITE_OP:
		case NAMESPACE_HANDLER_TRUNCATE_OP:
		case NAMESPACE_HANDLER_RENAME_OP:
			/*
			 * This handles the case of the resolver itself
			 * writing data to the file (or throwing it
			 * away).
			 */
			error = 0;
			break;
		case NAMESPACE_HANDLER_READ_OP:
			/*
			 * This handles the case of the resolver needing
			 * to look up inside of a dataless directory while
			 * it's in the process of materializing it (for
			 * example, creating files or directories).
			 */
			error = (vnode_vtype(vp) == VDIR) ? 0 : EBADF;
			break;
		default:
			error = EBADF;
			break;
		}
	}

	return error;
/*
 * 2. Call VNOP_READDIR() till the end of directory is reached.
 * 3. Check if the directory entries returned are regular files with name starting with "._".  If not, return ENOTEMPTY.
errno_t
rmdir_remove_orphaned_appleDouble(vnode_t vp, vfs_context_t ctx, int * restart_flag)
	char uio_buf[UIO_SIZEOF(1)];
	char *rbuf = NULL;
	void *dir_pos;
	void *dir_end;
	if (error == EBUSY) {
	}
	if (error != 0) {
		return error;
	}

	/*
	 * Prevent dataless fault materialization while we have
	 * a suspended vnode.
	 */
	uthread_t ut = get_bsdthread_info(current_thread());
	bool saved_nodatalessfaults =
	    (ut->uu_flag & UT_NSPACE_NODATALESSFAULTS) ? true : false;
	ut->uu_flag |= UT_NSPACE_NODATALESSFAULTS;
	if (rbuf) {
		    &uio_buf[0], sizeof(uio_buf));
	}
	uio_setoffset(auio, 0);
	if ((error = VNOP_OPEN(vp, FREAD, ctx))) {
		goto outsc;
	} else {
	}
		if ((error = VNOP_READDIR(vp, auio, 0, &eofflag, &nentries, ctx))) {
		}
		if (uio_resid(auio) != 0) {
		}
		dir_pos = (void*) rbuf;
		dir_end = (void*) (rbuf + siz);
		dp = (struct dirent*) (dir_pos);
		if (dir_pos == dir_end) {
		}
		while (dir_pos < dir_end) {
			if (dp->d_ino != 0 &&
			    !((dp->d_namlen == 1 && dp->d_name[0] == '.') ||
			    (dp->d_namlen == 2 && dp->d_name[0] == '.' && dp->d_name[1] == '.'))) {
				if (dp->d_namlen < 2 ||
				    strncmp(dp->d_name, "._", 2) ||
				    (dp->d_namlen >= 4 && !strncmp(&(dp->d_name[2]), "._", 2))) {
			dir_pos = (void*) ((uint8_t*)dir_pos + dp->d_reclen);
			dp = (struct dirent*)dir_pos;

		 * workaround for HFS/NFS setting eofflag before end of file
		if (vp->v_tag == VT_HFS && nentries > 2) {
			eofflag = 0;
		}
			} else if (!eofflag && full_erase_flag) {
			}
		if (error != 0) {
		}
		if (uio_resid(auio) != 0) {
		}
		dir_pos = (void*) rbuf;
		dir_end = (void*) (rbuf + siz);
		dp = (struct dirent*) dir_pos;

		if (dir_pos == dir_end) {
		}

		while (dir_pos < dir_end) {
			if (dp->d_ino != 0 &&
			    !((dp->d_namlen == 1 && dp->d_name[0] == '.') ||
			    (dp->d_namlen == 2 && dp->d_name[0] == '.' && dp->d_name[1] == '.'))
			    ) {
				error = unlink1(ctx, vp,
				    CAST_USER_ADDR_T(dp->d_name), UIO_SYSSPACE,
				    VNODE_REMOVE_SKIP_NAMESPACE_EVENT |
				    VNODE_REMOVE_NO_AUDIT_PATH);

				if (error && error != ENOENT) {
			dir_pos = (void*) ((uint8_t*)dir_pos + dp->d_reclen);
			dp = (struct dirent*)dir_pos;

		 * workaround for HFS/NFS setting eofflag before end of file
		if (vp->v_tag == VT_HFS && nentries > 2) {
			eofflag = 0;
		}
			} else if (!eofflag && full_erase_flag) {
			}
	if (open_flag) {
	}
	if (auio) {
		uio_free(auio);
	}
	if (saved_nodatalessfaults == false) {
		ut->uu_flag &= ~UT_NSPACE_NODATALESSFAULTS;
	}
	vnode_resume(vp);
	return error;
void
lock_vnode_and_post(vnode_t vp, int kevent_num)
void panic_print_vnodes(void);

/* define PANIC_PRINTS_VNODES only if investigation is required. */
#ifdef PANIC_PRINTS_VNODES

static const char *
__vtype(uint16_t vtype)
{
	switch (vtype) {
	case VREG:
		return "R";
	case VDIR:
		return "D";
	case VBLK:
		return "B";
	case VCHR:
		return "C";
	case VLNK:
		return "L";
	case VSOCK:
		return "S";
	case VFIFO:
		return "F";
	case VBAD:
		return "x";
	case VSTR:
		return "T";
	case VCPLX:
		return "X";
	default:
		return "?";
	}
}

/*
 * build a path from the bottom up
 * NOTE: called from the panic path - no alloc'ing of memory and no locks!
 */
static char *
__vpath(vnode_t vp, char *str, int len, int depth)
{
	int vnm_len;
	const char *src;
	char *dst;

	if (len <= 0) {
		return str;
	}
	/* str + len is the start of the string we created */
	if (!vp->v_name) {
		return str + len;
	}

	/* follow mount vnodes to get the full path */
	if ((vp->v_flag & VROOT)) {
		if (vp->v_mount != NULL && vp->v_mount->mnt_vnodecovered) {
			return __vpath(vp->v_mount->mnt_vnodecovered,
			           str, len, depth + 1);
		}
		return str + len;
	}

	src = vp->v_name;
	vnm_len = strlen(src);
	if (vnm_len > len) {
		/* truncate the name to fit in the string */
		src += (vnm_len - len);
		vnm_len = len;
	}

	/* start from the back and copy just characters (no NULLs) */

	/* this will chop off leaf path (file) names */
	if (depth > 0) {
		dst = str + len - vnm_len;
		memcpy(dst, src, vnm_len);
		len -= vnm_len;
	} else {
		dst = str + len;
	}

	if (vp->v_parent && len > 1) {
		/* follow parents up the chain */
		len--;
		*(dst - 1) = '/';
		return __vpath(vp->v_parent, str, len, depth + 1);
	}

	return dst;
}

#define SANE_VNODE_PRINT_LIMIT 5000
void
panic_print_vnodes(void)
{
	mount_t mnt;
	vnode_t vp;
	int nvnodes = 0;
	const char *type;
	char *nm;
	char vname[257];

	paniclog_append_noflush("\n***** VNODES *****\n"
	    "TYPE UREF ICNT PATH\n");

	/* NULL-terminate the path name */
	vname[sizeof(vname) - 1] = '\0';

	/*
	 * iterate all vnodelist items in all mounts (mntlist) -> mnt_vnodelist
	 */
	TAILQ_FOREACH(mnt, &mountlist, mnt_list) {
		if (!ml_validate_nofault((vm_offset_t)mnt, sizeof(mount_t))) {
			paniclog_append_noflush("Unable to iterate the mount list %p - encountered an invalid mount pointer %p \n",
			    &mountlist, mnt);
			break;
		}

		TAILQ_FOREACH(vp, &mnt->mnt_vnodelist, v_mntvnodes) {
			if (!ml_validate_nofault((vm_offset_t)vp, sizeof(vnode_t))) {
				paniclog_append_noflush("Unable to iterate the vnode list %p - encountered an invalid vnode pointer %p \n",
				    &mnt->mnt_vnodelist, vp);
				break;
			}

			if (++nvnodes > SANE_VNODE_PRINT_LIMIT) {
				return;
			}
			type = __vtype(vp->v_type);
			nm = __vpath(vp, vname, sizeof(vname) - 1, 0);
			paniclog_append_noflush("%s %0d %0d %s\n",
			    type, vp->v_usecount, vp->v_iocount, nm);
		}
	}
}

#else /* !PANIC_PRINTS_VNODES */
void
panic_print_vnodes(void)
{
	return;
}
#endif


static void
record_vp(vnode_t vp, int count)
{
	struct uthread *ut;
	if (vp->v_resolve) {
	}
	if ((vp->v_flag & VSYSTEM)) {
		return;
	}
	ut->uu_iocount += count;
	return (((uint64_t)seq) << 32) |
	       (((uint64_t)(aux & 0x0fffffff)) << 4) |
	       (uint64_t)(stat & 0x0000000F);
	return result & 0x0000000F;
		return EINVAL;
		return EINVAL;
		if (stat == RESOLVER_RESOLVED) {
		} else {
		}
	return 0;

	if (tinfo->vnt_data) {
	}
	if (rp == NULL) {
		return ENOMEM;
	}
	return result;
	mount_t mp;
	}
	/* Keep count of active trigger vnodes per mount */
	OSAddAtomic(-1, &mp->mnt_numtriggers);
		if (status == RESOLVER_UNRESOLVED) {
		}
		return 0;
		return 0;
#if CONFIG_MACF
	if ((rp->vr_flags & VNT_KERN_RESOLVE) == 0) {
		/*
		 * VNT_KERN_RESOLVE indicates this trigger has no parameters
		 * at the discression of the accessing process other than
		 * the act of access. All other triggers must be checked
		 */
		int rv = mac_vnode_check_trigger_resolve(ctx, vp, &ndp->ni_cnd);
		if (rv != 0) {
			return rv;
		}
	}
#endif

	 *
		if (status == RESOLVER_RESOLVED) {
		}
	return status == RESOLVER_ERROR ? vfs_resolver_auxiliary(result) : 0;
		return 0;
		return 0;
		if (status == RESOLVER_UNRESOLVED) {
		}
	return status == RESOLVER_ERROR ? vfs_resolver_auxiliary(result) : 0;
		if (mp->mnt_flag & MNT_ROOTFS) {
		}
		if (vp == NULLVP) {
		}
	return match;
	vfs_context_t   ctx;
	mount_t         top_mp;
	vnode_t         trigger_vp;
	mount_t         trigger_mp;
	uint32_t        trigger_vid;
	int             flags;
	if (mp == infop->top_mp) {
		return VFS_RETURNED_DONE;
	}
		return VFS_RETURNED;
	if (!mountedtrigger || !triggerisdescendant(mp, infop->top_mp)) {
		return VFS_RETURNED;
	}

			    mp->mnt_vfsstat.f_mntonname);
			return VFS_RETURNED;
			    infop->trigger_mp, vp->v_mountedhere);
			    vp->v_mountedhere ? vp->v_mountedhere->mnt_vfsstat.f_mntonname :
			    "???", error);
			return VFS_RETURNED_DONE; /* stop iteration on errors */
	return VFS_RETURNED;
		if (recursive) {
		}

	/*
	 * Must be called for trigger callback, wherein rwlock is held
	/*
	    CAST_USER_ADDR_T(relpath), ctx);

	/*

vm_offset_t
kdebug_vnode(vnode_t vp)
{
	return VM_KERNEL_ADDRPERM(vp);
}

static int flush_cache_on_write = 0;
SYSCTL_INT(_kern, OID_AUTO, flush_cache_on_write,
    CTLFLAG_RW | CTLFLAG_LOCKED, &flush_cache_on_write, 0,
    "always flush the drive cache on writes to uncached files");

int
vnode_should_flush_after_write(vnode_t vp, int ioflag)
{
	return flush_cache_on_write
	       && (ISSET(ioflag, IO_NOCACHE) || vnode_isnocache(vp));
}

/*
 * sysctl for use by disk I/O tracing tools to get the list of existing
 * vnodes' paths
 */

struct vnode_trace_paths_context {
	uint64_t count;
	long path[MAXPATHLEN / sizeof(long) + 1];  /* + 1 in case sizeof (long) does not divide MAXPATHLEN */
};

static int
vnode_trace_path_callback(struct vnode *vp, void *arg)
{
	int len, rv;
	struct vnode_trace_paths_context *ctx;

	ctx = arg;

	len = sizeof(ctx->path);
	rv = vn_getpath(vp, (char *)ctx->path, &len);
	/* vn_getpath() NUL-terminates, and len includes the NUL */

	if (!rv) {
		kdebug_vfs_lookup(ctx->path, len, vp,
		    KDBG_VFS_LOOKUP_FLAG_LOOKUP | KDBG_VFS_LOOKUP_FLAG_NOPROCFILT);

		if (++(ctx->count) == 1000) {
			thread_yield_to_preemption();
			ctx->count = 0;
		}
	}

	return VNODE_RETURNED;
}

static int
vfs_trace_paths_callback(mount_t mp, void *arg)
{
	if (mp->mnt_flag & MNT_LOCAL) {
		vnode_iterate(mp, VNODE_ITERATE_ALL, vnode_trace_path_callback, arg);
	}

	return VFS_RETURNED;
}

static int sysctl_vfs_trace_paths SYSCTL_HANDLER_ARGS {
	struct vnode_trace_paths_context ctx;

	(void)oidp;
	(void)arg1;
	(void)arg2;
	(void)req;

	if (!kauth_cred_issuser(kauth_cred_get())) {
		return EPERM;
	}

	if (!kdebug_enable || !kdebug_debugid_enabled(VFS_LOOKUP)) {
		return EINVAL;
	}

	bzero(&ctx, sizeof(struct vnode_trace_paths_context));

	vfs_iterate(0, vfs_trace_paths_callback, &ctx);

	return 0;
}

SYSCTL_PROC(_vfs_generic, OID_AUTO, trace_paths, CTLFLAG_RD | CTLFLAG_LOCKED | CTLFLAG_MASKED, NULL, 0, &sysctl_vfs_trace_paths, "-", "trace_paths");
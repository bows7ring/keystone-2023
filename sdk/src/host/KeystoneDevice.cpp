//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "KeystoneDevice.hpp"
#include <sys/mman.h>

namespace Keystone {

KeystoneDevice::KeystoneDevice() { eid = -1; }

Error
KeystoneDevice::create(uint64_t minPages, uintptr_t is_clone) {
  struct keystone_ioctl_create_enclave encl;
  encl.min_pages = minPages;
  encl.is_clone  = is_clone;

  if (ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &encl)) {
    perror("[create] ioctl error");
    eid = -1;
    return Error::IoctlErrorCreate;
  }

  eid      = encl.eid;
  physAddr = encl.pt_ptr;

  return Error::Success;
}

uintptr_t
KeystoneDevice::initUTM(size_t size) {
  struct keystone_ioctl_create_enclave encl;
  encl.eid                   = eid;
  encl.params.untrusted_size = size;
  if (ioctl(fd, KEYSTONE_IOC_UTM_INIT, &encl)) {
    return 0;
  }

  return encl.utm_free_ptr;
}

Error
KeystoneDevice::finalize(
    uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr,
    struct runtime_params_t params) {
  struct keystone_ioctl_create_enclave encl;
  encl.eid           = eid;
  encl.runtime_paddr = runtimePhysAddr;
  encl.user_paddr    = eappPhysAddr;
  encl.free_paddr    = freePhysAddr;
  encl.params        = params;

  if (ioctl(fd, KEYSTONE_IOC_FINALIZE_ENCLAVE, &encl)) {
    perror("[finalize] ioctl error");
    return Error::IoctlErrorFinalize;
  }
  return Error::Success;
}

Error
KeystoneDevice::clone_enclave(
    struct keystone_ioctl_clone_enclave encl) {
  encl.eid = eid;
  if (ioctl(fd, KEYSTONE_IOC_CLONE_ENCLAVE, &encl)) {
    perror("[clone] ioctl error");
    return Error::IoctlErrorFinalize;
  }
  return Error::Success;
}

Error
KeystoneDevice::destroy() {
  struct keystone_ioctl_create_enclave encl;
  encl.eid = eid;

  /* if the enclave has never created */
  if (eid < 0) {
    return Error::Success;
  }

  if (ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &encl)) {
    perror("[destroy] ioctl error");
    return Error::IoctlErrorDestroy;
  }

  return Error::Success;
}

Error
KeystoneDevice::destroySnapshot(uintptr_t snapshot_eid) {
  struct keystone_ioctl_create_enclave encl;
  encl.eid = snapshot_eid;

  /* if the snapshot has never created */
  if (eid < 0) {
    return Error::Success;
  }

  if (ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &encl)) {
    perror("[destorySnapshot] ioctl error");
    return Error::IoctlErrorDestroy;
  }

  return Error::Success;
}

Error
KeystoneDevice::__run(bool resume, uintptr_t* ret) {
  struct keystone_ioctl_run_enclave encl;
  encl.eid = eid;

  Error error;
  uint64_t request;

  if (resume) {
    error   = Error::IoctlErrorResume;
    request = KEYSTONE_IOC_RESUME_ENCLAVE;
  } else {
    error   = Error::IoctlErrorRun;
    request = KEYSTONE_IOC_RUN_ENCLAVE;
  }

  if (ioctl(fd, request, &encl)) {
    return error;
  }

  switch (encl.error) {
    case KEYSTONE_ENCLAVE_EDGE_CALL_HOST:
      return Error::EdgeCallHost;
    case KEYSTONE_ENCLAVE_INTERRUPTED:
      return Error::EnclaveInterrupted;
    case KEYSTONE_ENCLAVE_CLONE:
      return Error::EnclaveCloneRequested;
    case SBI_ERR_SM_ENCLAVE_SNAPSHOT:
      if (ret) {
        *ret = encl.value;
      }
      return Error::EnclaveSnapshot;
    case KEYSTONE_ENCLAVE_DONE:
      if (ret) {
        *ret = encl.value;
      }
      return Error::Success;
    default:
      ERROR(
          "Unknown SBI error (%d) returned by %s_enclave\n", encl.error,
          resume ? "resume" : "run");
      return error;
  }
}

Error
KeystoneDevice::run(uintptr_t* ret) {
  return __run(false, ret);
}

Error
KeystoneDevice::resume(uintptr_t* ret) {
  return __run(true, ret);
}

void*
KeystoneDevice::map(uintptr_t addr, size_t size) {
  assert(fd >= 0);
  void* ret;
  ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);
  assert(ret != MAP_FAILED);
  return ret;
}

bool
KeystoneDevice::initDevice() {
  /* open device driver */
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if (fd < 0) {
    PERROR("cannot open device file");
    return false;
  }
  return true;
}

Error
MockKeystoneDevice::create(uint64_t minPages) {
  eid = -1;
  return Error::Success;
}

uintptr_t
MockKeystoneDevice::initUTM(size_t size) {
  return 0;
}

Error
MockKeystoneDevice::finalize(
    uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr,
    struct runtime_params_t params) {
  return Error::Success;
}

Error
MockKeystoneDevice::destroy() {
  return Error::Success;
}

Error
MockKeystoneDevice::run(uintptr_t* ret) {
  return Error::Success;
}

Error
MockKeystoneDevice::resume(uintptr_t* ret) {
  return Error::Success;
}

bool
MockKeystoneDevice::initDevice() {
  return true;
}

void*
MockKeystoneDevice::map(uintptr_t addr, size_t size) {
  sharedBuffer = malloc(size);
  return sharedBuffer;
}

MockKeystoneDevice::~MockKeystoneDevice() {
  if (sharedBuffer) free(sharedBuffer);
}

}  // namespace Keystone

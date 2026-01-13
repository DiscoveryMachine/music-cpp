// Copyright (c) 2025 Discovery Machine®, Inc.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#ifndef MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_REQUEST_STATUS_H_
#define MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_REQUEST_STATUS_H_

namespace music {
enum class RequestStatus {
  // INVALID VALUE
  kUnset = -1,
  // The status of the Request is unknown.
  kOtherUnknown,
  // This means that the request has been received but hasn't been started yet.
  kPending,
  // This status means that the request is being performed.
  kExecuting,
  // This status means that the request has finished but wasn't able to fully
  // achieve the desired outcome.
  kPartiallyComplete,
  // This status means that the request has finished successfully.
  kComplete,
  // This status means that the request has failed to complete.
  kAborted
};
}  // namespace music

#endif  // MUSIC_DTO_CPP_MUSIC_INCLUDE_ENUM_REQUEST_STATUS_H_

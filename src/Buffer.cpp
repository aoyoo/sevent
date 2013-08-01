#include <Buffer.h>

#include <errno.h>
#include <sys/uio.h>

#include <Message.h>
#include <SocketsOps.h>
#include <Logger.h>

using namespace sevent;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;

int Buffer::getMessageNum(){
	int bufReadableBytes = readableBytes();
	int count = 0;
	while(1){
		if(bufReadableBytes < sizeof(MessageHead))
			return count;
		MessageHead head(peek());
		//head.ntoh(); //FIXME
		if(head.length < 0 || head.length > 1024*1024*10){ //TODO length == 0?
			LOG_ERROR(head.toString());
			return -1;  //get ERROR
		}
		if(bufReadableBytes - sizeof(MessageHead) < head.length){
			return count;
		}
		++count;
		bufReadableBytes = bufReadableBytes - sizeof(MessageHead) - head.length;
	}
}

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  // saved an ioctl()/FIONREAD call to tell how much to read
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin()+writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  const ssize_t n = sockets::readv(fd, vec, 2);
  if (n < 0)
  {
    *savedErrno = errno;
  }
  //else if (implicit_cast<size_t>(n) <= writable) //?? implicit_cast in <muduo/base/Types.h> is more safe ?
  else if (static_cast<size_t>(n) <= writable)
  {
    writerIndex_ += n;
  }
  else
  {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  // if (n == writable + sizeof extrabuf)
  // {
  //   goto line_30;
  // }
  return n;
}

ssize_t Buffer::readMessage(int fd, int* savedErrno)
{
  // saved an ioctl()/FIONREAD call to tell how much to read
  ensureWritableBytes(MESSAGE_HEAD_LEN);
  const size_t writable = writableBytes();

  const ssize_t n = sockets::read(fd, begin()+writerIndex_, MESSAGE_HEAD_LEN);
  if (n < 0)
  {
    *savedErrno = errno;
  }
  //else if (implicit_cast<size_t>(n) <= writable) //?? implicit_cast in <muduo/base/Types.h> is more safe ?
  else
  {
    writerIndex_ += n;
  }
  return n;
}




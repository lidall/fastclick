#ifndef POLLDEVICE_HH
#define POLLDEVICE_HH

/*
 * =c
 * PollDevice(devname)
 * =d
 * 
 * Poll packets received by the Linux network interface named devname.
 * Packets can be pulled from output 0. The packets include the link-level
 * header.
 *
 * Linux won't see any packets from the device. If you want Linux to process
 * packets, you should hand them to ToLinux.
 *
 * This element is only available inside the kernel module.
 *
 * =a FromDevice
 * =a ToDevice
 * =a ToLinux
 */

#include "element.hh"
#include "string.hh"
#include "glue.hh"

#define POLLDEV_IDLE_LIMIT 32
#define POLLDEV_MAX_PKTS_PER_RUN 8

class PollDevice : public Element {
 public:
  
  PollDevice();
  PollDevice(const String &);
  ~PollDevice();
  
  static void static_initialize();
  static void static_cleanup();
  
  const char *class_name() const		{ return "PollDevice"; }
  Processing default_processing() const		{ return PUSH; }
  
  PollDevice *clone() const;
  int configure(const String &, ErrorHandler *);
  int initialize(ErrorHandler *);
  void uninitialize();
  void add_handlers(HandlerRegistry *);
  
  /* process a packet. return 0 if not wanted after all. */
  int got_skb(struct sk_buff *);

  void run_scheduled();
 
  // statistics
  int _total_intr_wait;
  int _pkts_received;
  
 private:
  int _idle;
  String _devname;
  struct device *_dev;
  struct wait_queue _self_wq;

  static int num_polldevices;
  static int num_idle_polldevices;
};

#endif 


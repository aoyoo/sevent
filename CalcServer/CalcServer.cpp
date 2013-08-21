#include "CalcServer.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "proto_message.pb.h"
#include "i_dont_known_opencv.h"
#include "init.h"
#include "mask.h"
#include "myxmlwrap.h"

#include "ImageCommon.h"

using namespace sevent;
using namespace std;
CalcServer::CalcServer(const std::string name, const InetAddress & addr)
	:server_(name, addr), count(0)
{
	server_.setConnectionCallback(boost::bind(&CalcServer::onConnection, this, _1));
	server_.setMessageCallback(boost::bind(&CalcServer::taskFunc, this, _1, _2, _3));
}

CalcServer::~CalcServer()
{
	LOG_INFO("CalcServer destroy");
	cf_data_uninit();
	gf_data_uninit();
	lf_data_uninit();
	text_label_uninit();
	color_label_uninit();
	npx_uninit();
	image_quality_uninit();
	image_splicing_uninit();
	a_image_uninit();
	dominant_color_uninit();
	tm_judge_uninit();
	cf3_uninit();
}

int CalcServer::setup()
{
	LOG_INFO("CalcServer setup");

	//cf_data
	int cf_init_ret = cf_data_init(_cf_path);
	if (0 != cf_init_ret) {
		if (-1 == cf_init_ret) {
			LOG_WARN("cf_data init has not Center_Index");
		} else {
			LOG_ERROR("cf_data init error: " << cf_init_ret);
			return -1;
		}
	}
	//lf_data
	if (0 != lf_data_init(_lf_path)) {
		LOG_ERROR("lf_data init error");
		return -1;
	}
	//gf_data
	if (0 != gf_data_init(_gf_path)) {
		LOG_ERROR("gf_data init error");
		return -1;
	}
	//text_label
	if (0 != text_label_init(_tl_path)) {
		LOG_ERROR("tv init error");
		return -1;
	}
	//color_label
	color_label_init();

	//npx
	if (0 != npx_init(_npx_path)) {
		LOG_ERROR("npx init error");
		return -1;
	}
	//image_quality
	if (1 != image_quality_init(_iq_path)) {
		LOG_ERROR("image quality init error");
		return -1;
	}
	//image_splicing
	if (0 != image_splicing_init(_is_all_path, _is_shoe_path)) {
		LOG_ERROR("image splicing init error");
		return -1;
	}
	//a_image
	if (0 != a_image_init(_ai_path)) {
		LOG_ERROR("a_image init error");
		return -1;
	}
	//dominant_color
	//same as color_label
	if (0 != dominant_color_init()) {
		LOG_ERROR("dominant_image init error");
		return -1;
	}
	//tm_judge
	if (1 != tm_judge_init(_tj_path)) {
		LOG_ERROR("tm judge init error");
		return -1;
	}
	//cf3
	int cf3_init_ret = cf3_init(_cf3_path);
	if (0 != cf3_init_ret) {
		if (-1 == cf3_init_ret) {
			LOG_WARN("cf3 has not Center_Index");
		} else {
			LOG_ERROR("cf3 init error " << cf3_init_ret);
			return -1;
		}
	}
	//sf_data
	if (0 != sf_init(_sf_path)) {
		LOG_ERROR("sf_data init error");
		return -1;
	}
}

void CalcServer::onConnection(const ConnectionPtr & conn)
{
	LOG_INFO(conn->name() << (conn->connected()? " UP" : " DOWN"));
	if (conn->connected()) {
		conn->setTcpNoDelay(true);
	}
}

int CalcServer::taskFunc(const ConnectionPtr & c, const MessagePtr & msg, Timestamp time)
{
	LOG_DEBUG("CalcServer taskFunc start!");
	__sync_fetch_and_add(&count, 1);

	if (msg->head().command != 90001) {
		LOG_ERROR("command error " << msg->head().command);
		return -1;
	}

	proto_request req;
	if (!req.ParseFromString(msg->body())) {
		LOG_DEBUG("ParseFromString error");
		return -1;
	}

	int mask = req.mask();
	unsigned long long image_id = req.iid();
	int class_id = req.cid();

	if (req.has_image_data()) {
		LOG_INFO("calc has image data iid " << image_id << " cid %d " << class_id << " mask " << mask << " image_size " << req.image_data().size());
		int image_width = 0;
		int image_height = 0;

		if (mask & need_resize) {
			IplImage *img = cv_resize_image(const_cast < char *>(req.image_data().data()), req.image_data().size(), image_width, image_height);
			if (!img) {
				LOG_ERROR("resize image failed");
				return -1;
			}
			if (IsValidImage(img) != TOK) {
				LOG_ERROR("not validImage");
				cv_release_image(img);
				return -1;
			}

			if (cv_image_too_small(img)) {
				LOG_ERROR("image too small iid " << image_id);
				cv_release_image(img);
				return -1;
			}
			if (mask & global_data_mask) {
				LOG_INFO("global data start " << image_id);
				int ret;
				unsigned int buf_len = GLOBALFEATURE_4_0_0::GetFeatureValueLength();
				char data_buf[buf_len];
				int vid;
				ret = GLOBALFEATURE_4_0_0::extract(img, class_id, data_buf, vid);
				if (ret != 0) {
					LOG_ERROR("global data extract failed " << ret);
					return -1;
				}
			}
		}
	}

	LOG_DEBUG("CalcServer taskFunc over!");
	return 0;
}

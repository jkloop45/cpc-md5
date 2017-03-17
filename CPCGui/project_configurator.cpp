/**************************************************************************\
|
|    Copyright (C) 2009 Marc Stevens
|
|    This program is free software: you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation, either version 3 of the License, or
|    (at your option) any later version.
|
|    This program is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with this program.  If not, see <http://www.gnu.org/licenses/>.
|
\**************************************************************************/

#include <boost/lexical_cast.hpp>
#include "project_configurator.hpp"

extern double dist[33][8][17];
template<class T>
T* manage_sizegroup(Glib::RefPtr<Gtk::SizeGroup> sizegroup, T* obj) {
	sizegroup->add_widget(*obj);
	return Gtk::manage(obj);
}
template<class T>
T& add_sizegroup(Glib::RefPtr<Gtk::SizeGroup> sizegroup, T& obj) {
	sizegroup->add_widget(obj);
	return obj;
}

BirthdaySearchOptions::~BirthdaySearchOptions() {}
BirthdaySearchOptions::BirthdaySearchOptions(birthdaysearch_config_type& birthdaysearchconfig
											 , Glib::RefPtr<Gtk::SizeGroup> sizegroup_labels
											 , Glib::RefPtr<Gtk::SizeGroup> sizegroup_options)
	: birthdaysearch_config(birthdaysearchconfig)
	, table_options(9, 2, false)
{
	set_label("Birthday search options");
	set_border_width(10);
	table_options.set_border_width(10);
	table_options.set_spacings(3);
	table_options.set_col_spacings(10);
	int row = 0;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Hybrid bits", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Path type range", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum # blocks", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum # MB memory to use", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Hard memory limit", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Traillength in bits", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Use NVIDIA GPU", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Use CELL CPU", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("# CELL SPU threads", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Estimated complexity", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Estimated cputime", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	row = 0;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_hybridbits), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_path_type_range), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_blocks), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_memory), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_mem_hardlimit), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_log_traillength), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_use_cuda), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_use_cell), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_cell_nr_spu_threads), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(label_complexity, 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(label_cpuhours, 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	label_complexity.set_sensitive(false);
	label_cpuhours.set_sensitive(false);
	load_changes();
	cfg_hybridbits.signal_value_changed().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_path_type_range.signal_value_changed().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_max_blocks.signal_value_changed().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_max_memory.signal_value_changed().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_cell_nr_spu_threads.signal_value_changed().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_log_traillength.signal_value_changed().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_mem_hardlimit.signal_toggled().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_use_cuda.signal_toggled().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	cfg_use_cell.signal_toggled().connect( sigc::mem_fun(*this, &BirthdaySearchOptions::save_changes) );
	add(table_options);
}
void BirthdaySearchOptions::update_estimations()
{
	if (birthdaysearch_config.max_memory == 0 && birthdaysearch_config.log_traillength == -1) {
		birthdaysearch_config.max_memory = 1;
		cfg_max_memory.set_value(1);
	}
	boost::uint64_t maxtrails = 0;
	unsigned maxblocks = birthdaysearch_config.max_blocks;
	if (maxblocks > 16)
		maxblocks = 16;
	double logprob = log(dist[birthdaysearch_config.hybridbits][birthdaysearch_config.path_type_range][maxblocks])/log(double(2));
	if (maxblocks == 1) {
		logprob = (log(double(
			10504191 // singleblockdata.size()
			))/log(2.0)) - (64 - birthdaysearch_config.hybridbits);
	}
	double estcomplexity = 32.825748 - 0.5*logprob + 0.5*double(birthdaysearch_config.hybridbits);
	double estcollisions = 1-logprob;

	if (birthdaysearch_config.max_memory != 0)
	{
		maxtrails = boost::uint64_t(birthdaysearch_config.max_memory) << 19;
		maxtrails /= 28;
	}
	int logpathlength = birthdaysearch_config.log_traillength;
	if (logpathlength < 0)
	{
		double comppertrail = pow(double(2),estcomplexity)/double(maxtrails);
		double logcpt = log(comppertrail)/log(double(2));
		logpathlength = unsigned(logcpt+0.9);
	}
	double estcollcomplexity = logpathlength + estcollisions + 1.321928;
	double esttotcomplexity = log(pow(double(2),estcomplexity) + pow(double(2),estcollcomplexity))/log(2.0);
	double esttotcomplexity2 = log(pow(double(2),estcomplexity+0.5) + pow(double(2),estcollcomplexity+1))/log(2.0);
	if (birthdaysearch_config.max_memory != 0 && birthdaysearch_config.log_traillength >= 0) {
		if (maxtrails < boost::uint64_t(pow(double(2),estcomplexity-logpathlength))) {
			birthdaysearch_config.max_memory = (boost::uint64_t(pow(double(2),estcomplexity-logpathlength)) * boost::uint64_t(28)) >> 18;
			cfg_max_memory.set_value(birthdaysearch_config.max_memory);
			update_estimations();
			return;
		}
	}
	label_complexity.set_text("2^(" 
		+ boost::lexical_cast<std::string>(int(esttotcomplexity*10.0)/10) + "." + boost::lexical_cast<std::string>(int(esttotcomplexity*10.0)%10)
		+ ")"
		+ " [2^(" 
		+ boost::lexical_cast<std::string>(int(esttotcomplexity2*10.0)/10) + "." + boost::lexical_cast<std::string>(int(esttotcomplexity2*10.0)%10)
		+ ")]"
		);
	std::string cputime;
	{
		boost::uint64_t minutes = pow(double(2),esttotcomplexity-28.1);
		boost::uint64_t hours = minutes / 60; minutes -= hours*60;
		boost::uint64_t days = hours / 24; hours -= days * 24;
		if (days)
			cputime += boost::lexical_cast<std::string>(days) + " days ";
		if (days || hours)
			cputime += boost::lexical_cast<std::string>(hours) + "h ";
		cputime += boost::lexical_cast<std::string>(minutes) + "m";
	}
	{
		cputime += " [";
		boost::uint64_t minutes = pow(double(2),esttotcomplexity2-28.1);
		boost::uint64_t hours = minutes / 60; minutes -= hours*60;
		boost::uint64_t days = hours / 24; hours -= days * 24;
		if (days)
			cputime += boost::lexical_cast<std::string>(days) + " days ";
		if (days || hours)
			cputime += boost::lexical_cast<std::string>(hours) + "h ";
		cputime += boost::lexical_cast<std::string>(minutes) + "m";
		cputime += "]";
	}
	label_cpuhours.set_text(cputime);
}
void BirthdaySearchOptions::save_changes() {
	birthdaysearch_config.hybridbits = cfg_hybridbits.get_value_as_int();
	birthdaysearch_config.path_type_range = cfg_path_type_range.get_value_as_int();
	birthdaysearch_config.max_blocks = cfg_max_blocks.get_value_as_int();
	birthdaysearch_config.max_memory = cfg_max_memory.get_value_as_int();
	birthdaysearch_config.mem_hardlimit = cfg_mem_hardlimit.get_active();
	birthdaysearch_config.log_traillength = cfg_log_traillength.get_value_as_int();
	birthdaysearch_config.use_cuda = cfg_use_cuda.get_active();
	birthdaysearch_config.use_cell = cfg_use_cell.get_active();
	birthdaysearch_config.cell_nr_spu_threads = cfg_cell_nr_spu_threads.get_value_as_int();
	update_estimations();
}
void BirthdaySearchOptions::load_changes() {
	cfg_hybridbits.set_numeric();
	cfg_path_type_range.set_numeric();
	cfg_max_blocks.set_numeric();
	cfg_max_memory.set_numeric();
	cfg_log_traillength.set_numeric();
	cfg_cell_nr_spu_threads.set_numeric();
	cfg_hybridbits.get_adjustment()->configure(birthdaysearch_config.hybridbits, 0, 32, 1, 1, 0);		
	cfg_path_type_range.get_adjustment()->configure(birthdaysearch_config.path_type_range, 0, 32, 1, 1, 0);		
	cfg_max_blocks.get_adjustment()->configure(birthdaysearch_config.max_blocks, 1, 16, 1, 1, 0);
	cfg_max_memory.get_adjustment()->configure(birthdaysearch_config.max_memory, 0, (1<<20), 1, 20, 0);		
	cfg_mem_hardlimit.set_active(birthdaysearch_config.mem_hardlimit);
	cfg_log_traillength.get_adjustment()->configure(birthdaysearch_config.log_traillength, -1, 64, 1, 1, 0);
	cfg_use_cuda.set_active(birthdaysearch_config.use_cuda);
	cfg_use_cell.set_active(birthdaysearch_config.use_cell);
	cfg_cell_nr_spu_threads.get_adjustment()->configure(birthdaysearch_config.cell_nr_spu_threads, 0, 8, 1, 1, 0);
	update_estimations();
}




DiffPathConnectOptions::~DiffPathConnectOptions() {}
DiffPathConnectOptions::DiffPathConnectOptions(diffpathconnect_config_type& config
											   , Glib::RefPtr<Gtk::SizeGroup> sizegroup_labels
											   , Glib::RefPtr<Gtk::SizeGroup> sizegroup_options)
	: diffpathconnect_config(config)
	, table_options(6, 2, false)
{
	set_label("Diff.Path Connect options");
	set_border_width(10);
	table_options.set_border_width(10);
	table_options.set_spacings(3);
	table_options.set_col_spacings(10);
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Connect beginstep t=", Gtk::ALIGN_LEFT)), 0,1, 0,1, Gtk::FILL, Gtk::FILL);
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Begin Q_t to count conditions", Gtk::ALIGN_LEFT)), 0,1, 1,2, Gtk::FILL, Gtk::FILL);
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum complexity", Gtk::ALIGN_LEFT)), 0,1, 2,3, Gtk::FILL, Gtk::FILL);
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Do not verify diff.paths", Gtk::ALIGN_LEFT)), 0,1, 3,4, Gtk::FILL, Gtk::FILL);
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Do not enhance diff.paths", Gtk::ALIGN_LEFT)), 0,1, 4,5, Gtk::FILL, Gtk::FILL);
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Show statistics", Gtk::ALIGN_LEFT)), 0,1, 5,6, Gtk::FILL, Gtk::FILL);
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Abort after # cpucore minutes", Gtk::ALIGN_LEFT)), 0,1, 6,7, Gtk::FILL, Gtk::FILL);
	table_options.attach(add_sizegroup(sizegroup_options, cfg_t_step), 1,2, 0,1, Gtk::FILL, Gtk::FILL);
	table_options.attach(add_sizegroup(sizegroup_options, cfg_Qcond_start), 1,2, 1,2, Gtk::FILL, Gtk::FILL);
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_complexity), 1,2, 2,3, Gtk::FILL, Gtk::FILL);
	table_options.attach(add_sizegroup(sizegroup_options, cfg_no_verify), 1,2, 3,4, Gtk::FILL, Gtk::FILL);
	table_options.attach(add_sizegroup(sizegroup_options, cfg_no_enhance_path), 1,2, 4,5, Gtk::FILL, Gtk::FILL);
	table_options.attach(add_sizegroup(sizegroup_options, cfg_show_statistics), 1,2, 5,6, Gtk::FILL, Gtk::FILL);
	table_options.attach(add_sizegroup(sizegroup_options, cfg_abort_after_X_min), 1,2, 6,7, Gtk::FILL, Gtk::FILL);
	load_changes();
	cfg_t_step.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathConnectOptions::save_changes) );
	cfg_Qcond_start.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathConnectOptions::save_changes) );
	cfg_max_complexity.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathConnectOptions::save_changes) );
	cfg_no_verify.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathConnectOptions::save_changes) );
	cfg_no_enhance_path.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathConnectOptions::save_changes) );
	cfg_show_statistics.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathConnectOptions::save_changes) );
	cfg_abort_after_X_min.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathConnectOptions::save_changes) );
	add(table_options);
}
void DiffPathConnectOptions::save_changes() {
	diffpathconnect_config.t_step = cfg_t_step.get_value_as_int();
	diffpathconnect_config.Qcond_start = cfg_Qcond_start.get_value_as_int();
	diffpathconnect_config.max_complexity = cfg_max_complexity.get_value_as_int();
	diffpathconnect_config.no_verify = cfg_no_verify.get_active();
	diffpathconnect_config.no_enhance_path = cfg_no_enhance_path.get_active();
	diffpathconnect_config.show_statistics = cfg_show_statistics.get_active();
	diffpathconnect_config.abortafterXmin = cfg_abort_after_X_min.get_value_as_int();
}
void DiffPathConnectOptions::load_changes() {
	cfg_t_step.set_numeric();
	cfg_Qcond_start.set_numeric();
	cfg_max_complexity.set_numeric();
	cfg_abort_after_X_min.set_numeric();
	cfg_t_step.get_adjustment()->configure(diffpathconnect_config.t_step, 1, 16, 1, 1, 0);		
	cfg_Qcond_start.get_adjustment()->configure(diffpathconnect_config.Qcond_start, -3, 64, 1, 1, 0);		
	cfg_max_complexity.get_adjustment()->configure(diffpathconnect_config.max_complexity, -9999, 9999, 1, 10, 0);
	cfg_abort_after_X_min.get_adjustment()->configure(diffpathconnect_config.abortafterXmin, -1, 9999, 1, 5, 0);
	cfg_no_verify.set_active(diffpathconnect_config.no_verify);
	cfg_no_enhance_path.set_active(diffpathconnect_config.no_enhance_path);
	cfg_show_statistics.set_active(diffpathconnect_config.show_statistics);
}




DiffPathForwardOptions::~DiffPathForwardOptions() {}
DiffPathForwardOptions::DiffPathForwardOptions(diffpathforward_config_type& config
											   , Glib::RefPtr<Gtk::SizeGroup> sizegroup_labels
											   , Glib::RefPtr<Gtk::SizeGroup> sizegroup_options)
	: diffpathforward_config(config)
	, table_options(16, 2, false)
	, chainparent(0)
{
	set_label("Diff.Path Forward options");
	set_border_width(10);
	table_options.set_border_width(10);
	table_options.set_spacings(3);
	table_options.set_col_spacings(10);
	int row = 0;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("First step t=", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Last step t=", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum # conditions", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Begin Q_t to count conditions", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Include NAF weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Half NAF weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Minimum SDR weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1,Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum SDR weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum # SDRs", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: # diff.paths", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: estimate factor", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: NAF weight estimate", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: fill fraction", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Minimum Q_{4,5,6} tunnelstrength", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Minimum Q_{9,10,11} tunnelstrength", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Minimum Q_{3,14} tunnelstrength", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Do not process t=0,1 brute force", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Do not verify diff.paths", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	row = 0;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_t_step), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_t_range), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_conditions), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_cond_tbegin), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_include_naf), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_half_naf_weight), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_min_sdr_weight), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_sdr_weight), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_sdrs), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_ab_nr_diffpaths), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_ab_estimate_factor), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_ab_naf_estimate), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_fill_fraction), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_min_Q456_tunnel), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_min_Q91011_tunnel), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_min_Q314_tunnel), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_normalt01), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_no_verify), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	load_changes();
	cfg_t_step.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes_trange) );
	cfg_t_range.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes_trange) );
	cfg_max_conditions.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_min_sdr_weight.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_max_sdr_weight.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_max_sdrs.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_ab_nr_diffpaths.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_ab_naf_estimate.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_min_Q456_tunnel.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_min_Q91011_tunnel.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_min_Q314_tunnel.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_cond_tbegin.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_fill_fraction.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_ab_estimate_factor.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_include_naf.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_half_naf_weight.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_no_verify.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	cfg_normalt01.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathForwardOptions::save_changes) );
	add(table_options);
}
void DiffPathForwardOptions::save_changes_trange() {
	diffpathforward_config.t_step = cfg_t_step.get_value_as_int();
	diffpathforward_config.t_range = cfg_t_range.get_value_as_int()- diffpathforward_config.t_step;
	cfg_t_range.get_adjustment()->configure(diffpathforward_config.t_range + diffpathforward_config.t_step, diffpathforward_config.t_step, 79, 1, 1, 0);
	if (chainparent) chainparent->update_tabs();
}
void DiffPathForwardOptions::save_changes() {
	diffpathforward_config.max_conditions = cfg_max_conditions.get_value_as_int();
	diffpathforward_config.min_sdr_weight = cfg_min_sdr_weight.get_value_as_int();
	diffpathforward_config.max_sdr_weight = cfg_max_sdr_weight.get_value_as_int();
	diffpathforward_config.max_sdrs = cfg_max_sdrs.get_value_as_int();
	diffpathforward_config.ab_nr_diffpaths = cfg_ab_nr_diffpaths.get_value_as_int();
	diffpathforward_config.ab_naf_estimate = cfg_ab_naf_estimate.get_value_as_int();
	diffpathforward_config.min_Q456_tunnel = cfg_min_Q456_tunnel.get_value_as_int();
	diffpathforward_config.min_Q91011_tunnel = cfg_min_Q91011_tunnel.get_value_as_int();
	diffpathforward_config.min_Q314_tunnel = cfg_min_Q314_tunnel.get_value_as_int();
	diffpathforward_config.cond_tbegin = cfg_cond_tbegin.get_value_as_int();
	diffpathforward_config.fill_fraction = cfg_fill_fraction.get_value();
	diffpathforward_config.ab_estimate_factor = cfg_ab_estimate_factor.get_value();
	diffpathforward_config.include_naf = cfg_include_naf.get_active(); 
	diffpathforward_config.half_naf_weight = cfg_half_naf_weight.get_active(); 
	diffpathforward_config.no_verify = cfg_no_verify.get_active(); 
	diffpathforward_config.normalt01 = cfg_normalt01.get_active(); 	
}

void DiffPathForwardOptions::load_changes() {
	cfg_t_step.set_numeric();
	cfg_t_range.set_numeric();
	cfg_max_conditions.set_numeric();
	cfg_min_sdr_weight.set_numeric();
	cfg_max_sdr_weight.set_numeric();
	cfg_max_sdrs.set_numeric();
	cfg_ab_nr_diffpaths.set_numeric();
	cfg_ab_naf_estimate.set_numeric();
	cfg_min_Q456_tunnel.set_numeric();
	cfg_min_Q91011_tunnel.set_numeric();
	cfg_min_Q314_tunnel.set_numeric();	
	cfg_cond_tbegin.set_numeric();
	cfg_fill_fraction.set_numeric();
	cfg_ab_estimate_factor.set_numeric();
	cfg_t_step.get_adjustment()->configure(diffpathforward_config.t_step, 0, 79, 1, 1, 0);
	cfg_t_range.get_adjustment()->configure(diffpathforward_config.t_range + diffpathforward_config.t_step, diffpathforward_config.t_step, 79, 1, 1, 0);
	cfg_max_conditions.get_adjustment()->configure(diffpathforward_config.max_conditions, 0, 9999, 1, 1, 0);
	cfg_min_sdr_weight.get_adjustment()->configure(diffpathforward_config.min_sdr_weight, 0, 32, 1, 1, 0);
	cfg_max_sdr_weight.get_adjustment()->configure(diffpathforward_config.max_sdr_weight, 1, 32, 1, 1, 0);
	cfg_max_sdrs.get_adjustment()->configure(diffpathforward_config.max_sdrs, 1, (1<<20), 1, 1, 0);
	cfg_ab_nr_diffpaths.get_adjustment()->configure(diffpathforward_config.ab_nr_diffpaths, 0, (1<<30), 1, 1, 0);
	cfg_ab_naf_estimate.get_adjustment()->configure(diffpathforward_config.ab_naf_estimate, 0, 32, 1, 1, 0);
	cfg_min_Q456_tunnel.get_adjustment()->configure(diffpathforward_config.min_Q456_tunnel, 0, 32, 1, 1, 0);
	cfg_min_Q91011_tunnel.get_adjustment()->configure(diffpathforward_config.min_Q91011_tunnel, 0, 32, 1, 1, 0);
	cfg_min_Q314_tunnel.get_adjustment()->configure(diffpathforward_config.min_Q314_tunnel, 0, 32, 1, 1, 0);	
	cfg_cond_tbegin.get_adjustment()->configure(diffpathforward_config.cond_tbegin, -3, 64, 1, 1, 0);
	cfg_fill_fraction.set_digits(2);
	cfg_fill_fraction.get_adjustment()->configure(diffpathforward_config.fill_fraction, 0, 1, 0.01, 0.1, 0);
	cfg_ab_estimate_factor.set_digits(2);
	cfg_ab_estimate_factor.get_adjustment()->configure(diffpathforward_config.ab_estimate_factor, 0, 99, 0.1, 1, 0);
	cfg_include_naf.set_active(diffpathforward_config.include_naf); 
	cfg_half_naf_weight.set_active(diffpathforward_config.half_naf_weight); 
	cfg_no_verify.set_active(diffpathforward_config.no_verify); 
	cfg_normalt01.set_active(diffpathforward_config.normalt01); 	
}




DiffPathBackwardOptions::~DiffPathBackwardOptions() {}
DiffPathBackwardOptions::DiffPathBackwardOptions(diffpathbackward_config_type& config
												 , Glib::RefPtr<Gtk::SizeGroup> sizegroup_labels
												 , Glib::RefPtr<Gtk::SizeGroup> sizegroup_options)
	: diffpathbackward_config(config)
	, table_options(13, 2, false)
{
	set_label("Diff.Path Backward options");
	set_border_width(10);
	table_options.set_border_width(10);
	table_options.set_spacings(3);
	table_options.set_col_spacings(10);
	int row = 0;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("First step t=", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Last step t=", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum # conditions", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum # conditions from Q_26 up", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("End Q_t to count conditions", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Include NAF weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Half NAF weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Minimum SDR weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum SDR weight", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Maximum # SDRs", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: # diff.paths", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: estimate factor", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: NAF weight estimate", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Autobalance: fill fraction", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Do not verify diff.paths", Gtk::ALIGN_LEFT)), 0,1, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	row = 0;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_t_step), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_t_range), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_conditions), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_Q26up_cond), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_cond_tend), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_include_naf), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_half_naf_weight), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_min_sdr_weight), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_sdr_weight), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_max_sdrs), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_ab_nr_diffpaths), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_ab_estimate_factor), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_ab_naf_estimate), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_fill_fraction), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	table_options.attach(add_sizegroup(sizegroup_options, cfg_no_verify), 1,2, row,row+1, Gtk::FILL, Gtk::FILL);++row;
	load_changes();
	cfg_t_step.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes_trange) );
	cfg_t_range.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes_trange) );
	cfg_max_conditions.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_min_sdr_weight.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_max_sdr_weight.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_max_sdrs.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_ab_nr_diffpaths.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_ab_naf_estimate.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_max_Q26up_cond.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_cond_tend.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_fill_fraction.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_ab_estimate_factor.signal_value_changed().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_include_naf.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_half_naf_weight.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	cfg_no_verify.signal_toggled().connect( sigc::mem_fun(*this, &DiffPathBackwardOptions::save_changes) );
	add(table_options);
}
void DiffPathBackwardOptions::save_changes_trange() {
	diffpathbackward_config.t_step = cfg_t_step.get_value_as_int();
	diffpathbackward_config.t_range = diffpathbackward_config.t_step - cfg_t_range.get_value_as_int();
	cfg_t_range.get_adjustment()->configure(diffpathbackward_config.t_step - diffpathbackward_config.t_range, 0, diffpathbackward_config.t_step, 1, 1, 0);
	if (chainparent) chainparent->update_tabs();
}
void DiffPathBackwardOptions::save_changes() {
	diffpathbackward_config.max_conditions = cfg_max_conditions.get_value_as_int();
	diffpathbackward_config.max_Q26up_cond = cfg_max_Q26up_cond.get_value_as_int();
	diffpathbackward_config.min_sdr_weight = cfg_min_sdr_weight.get_value_as_int();
	diffpathbackward_config.max_sdr_weight = cfg_max_sdr_weight.get_value_as_int();
	diffpathbackward_config.max_sdrs = cfg_max_sdrs.get_value_as_int();
	diffpathbackward_config.ab_nr_diffpaths = cfg_ab_nr_diffpaths.get_value_as_int();
	diffpathbackward_config.ab_naf_estimate = cfg_ab_naf_estimate.get_value_as_int();
	diffpathbackward_config.cond_tend = cfg_cond_tend.get_value_as_int();
	diffpathbackward_config.fill_fraction = cfg_fill_fraction.get_value();
	diffpathbackward_config.ab_estimate_factor = cfg_ab_estimate_factor.get_value();
	diffpathbackward_config.include_naf = cfg_include_naf.get_active(); 
	diffpathbackward_config.half_naf_weight = cfg_half_naf_weight.get_active(); 
	diffpathbackward_config.no_verify = cfg_no_verify.get_active(); 	
}
void DiffPathBackwardOptions::load_changes() {
	cfg_t_step.set_numeric();
	cfg_t_range.set_numeric();
	cfg_max_conditions.set_numeric();
	cfg_max_Q26up_cond.set_numeric();
	cfg_min_sdr_weight.set_numeric();
	cfg_max_sdr_weight.set_numeric();
	cfg_max_sdrs.set_numeric();
	cfg_ab_nr_diffpaths.set_numeric();
	cfg_ab_naf_estimate.set_numeric();
	cfg_cond_tend.set_numeric();
	cfg_fill_fraction.set_numeric();
	cfg_ab_estimate_factor.set_numeric();
	cfg_t_step.get_adjustment()->configure(diffpathbackward_config.t_step, 0, 79, 1, 1, 0);
	cfg_t_range.get_adjustment()->configure(diffpathbackward_config.t_step - diffpathbackward_config.t_range, 0, diffpathbackward_config.t_step, 1, 1, 0);
	cfg_max_conditions.get_adjustment()->configure(diffpathbackward_config.max_conditions, 0, 9999, 1, 1, 0);
	cfg_max_Q26up_cond.get_adjustment()->configure(diffpathbackward_config.max_Q26up_cond, 0, 9999, 1, 1, 0);
	cfg_min_sdr_weight.get_adjustment()->configure(diffpathbackward_config.min_sdr_weight, 0, 32, 1, 1, 0);
	cfg_max_sdr_weight.get_adjustment()->configure(diffpathbackward_config.max_sdr_weight, 1, 32, 1, 1, 0);
	cfg_max_sdrs.get_adjustment()->configure(diffpathbackward_config.max_sdrs, 1, (1<<20), 1, 1, 0);
	cfg_ab_nr_diffpaths.get_adjustment()->configure(diffpathbackward_config.ab_nr_diffpaths, 0, (1<<30), 1, 1, 0);
	cfg_ab_naf_estimate.get_adjustment()->configure(diffpathbackward_config.ab_naf_estimate, 0, 32, 1, 1, 0);
	cfg_cond_tend.get_adjustment()->configure(diffpathbackward_config.cond_tend, -3, 64, 1, 1, 0);
	cfg_fill_fraction.set_digits(2);
	cfg_fill_fraction.get_adjustment()->configure(diffpathbackward_config.fill_fraction, 0, 1, 0.01, 0.1, 0);
	cfg_ab_estimate_factor.set_digits(2);
	cfg_ab_estimate_factor.get_adjustment()->configure(diffpathbackward_config.ab_estimate_factor, 0, 99, 0.1, 1, 0);
	cfg_include_naf.set_active(diffpathbackward_config.include_naf); 
	cfg_half_naf_weight.set_active(diffpathbackward_config.half_naf_weight); 
	cfg_no_verify.set_active(diffpathbackward_config.no_verify); 
}




DiffPathForwardChainOptions::~DiffPathForwardChainOptions() {}
DiffPathForwardChainOptions::DiffPathForwardChainOptions(std::vector<diffpathforward_config_type>& chain_config
	, Glib::RefPtr<Gtk::SizeGroup> sizegroup_labels, Glib::RefPtr<Gtk::SizeGroup> sizegroup_options)
	: buttonbox(Gtk::BUTTONBOX_START, 10)
	, button_add("Add page")
	, button_delete("Delete current page")
	, vec_diffpathforward_config(chain_config)
	, sg_labels(sizegroup_labels)
	, sg_options(sizegroup_options)
{
	set_spacing(10);
	set_border_width(10);
	pack_start(buttonbox, false, true);
	pack_start(notebook, true, true);
	notebook.set_tab_pos(Gtk::POS_LEFT);
	buttonbox.add(button_add);
	buttonbox.add(button_delete);
	button_add.signal_clicked().connect( sigc::mem_fun(*this, &DiffPathForwardChainOptions::add_page) );
	button_delete.signal_clicked().connect( sigc::mem_fun(*this, &DiffPathForwardChainOptions::delete_page) );
	fill_notebook();
}

void DiffPathForwardChainOptions::update_tabs()
{
	for (unsigned i = 0; i < vec_diffpathforward_options.size(); ++i) {
		if (vec_diffpathforward_config[i].t_range == 0) {
			notebook.pages()[i].set_tab_label_text(boost::lexical_cast<std::string>(vec_diffpathforward_config[i].t_step));
		} else {
			notebook.pages()[i].set_tab_label_text(
				boost::lexical_cast<std::string>(vec_diffpathforward_config[i].t_step) + "-" 
				+ boost::lexical_cast<std::string>(vec_diffpathforward_config[i].t_step + vec_diffpathforward_config[i].t_range)
				);
		}
	}
}
void DiffPathForwardChainOptions::load_all_changes()
{
	for (unsigned i = 0; i < vec_diffpathforward_options.size(); ++i)
		vec_diffpathforward_options[i]->load_all_changes();
}
void DiffPathForwardChainOptions::add_page()
{
	clear_notebook();
	vec_diffpathforward_config.push_back( diffpathforward_config_type() );
	if (vec_diffpathforward_config.size() > 1) {
		int i = vec_diffpathforward_config.size()-2;
		vec_diffpathforward_config.back().t_step = vec_diffpathforward_config[i].t_step + vec_diffpathforward_config[i].t_range + 1;
	}
	fill_notebook();
}
void DiffPathForwardChainOptions::delete_page()
{
	int k = notebook.get_current_page();
	clear_notebook();
	vec_diffpathforward_config.erase( vec_diffpathforward_config.begin()+k );
	fill_notebook();
}
void DiffPathForwardChainOptions::clear_notebook()
{
	notebook.pages().clear();
	for (unsigned i = 0; i < vec_diffpathforward_options.size(); ++i)
		delete vec_diffpathforward_options[i];
	vec_diffpathforward_options.clear();
}
void DiffPathForwardChainOptions::fill_notebook()
{
	for (unsigned i = 0; i < vec_diffpathforward_config.size(); ++i) {
		vec_diffpathforward_options.push_back(new DiffPathForwardOptions(vec_diffpathforward_config[i], sg_labels, sg_options));
		notebook.append_page(*vec_diffpathforward_options[i], "");
	}
	for (unsigned i = 0; i < vec_diffpathforward_config.size(); ++i)
		vec_diffpathforward_options[i]->set_chainparent(this);
	show_all();
	if (vec_diffpathforward_options.size())
		notebook.set_current_page(0);
	update_tabs();
}





DiffPathBackwardChainOptions::~DiffPathBackwardChainOptions() {}
DiffPathBackwardChainOptions::DiffPathBackwardChainOptions(std::vector<diffpathbackward_config_type>& chain_config
	, Glib::RefPtr<Gtk::SizeGroup> sizegroup_labels, Glib::RefPtr<Gtk::SizeGroup> sizegroup_options)
	: buttonbox(Gtk::BUTTONBOX_START, 10)
	, button_add("Add page")
	, button_delete("Delete current page")
	, vec_diffpathbackward_config(chain_config)
	, sg_labels(sizegroup_labels)
	, sg_options(sizegroup_options)
{
	set_spacing(10);
	set_border_width(10);
	pack_start(buttonbox, false, true);
	pack_start(notebook, true, true);
	notebook.set_tab_pos(Gtk::POS_LEFT);
	buttonbox.add(button_add);
	buttonbox.add(button_delete);
	button_add.signal_clicked().connect( sigc::mem_fun(*this, &DiffPathBackwardChainOptions::add_page) );
	button_delete.signal_clicked().connect( sigc::mem_fun(*this, &DiffPathBackwardChainOptions::delete_page) );
	fill_notebook();
}

void DiffPathBackwardChainOptions::update_tabs()
{
	for (unsigned i = 0; i < vec_diffpathbackward_options.size(); ++i) {
		if (vec_diffpathbackward_config[i].t_range == 0) {
			notebook.pages()[i].set_tab_label_text(boost::lexical_cast<std::string>(vec_diffpathbackward_config[i].t_step));
		} else {
			notebook.pages()[i].set_tab_label_text(
				boost::lexical_cast<std::string>(vec_diffpathbackward_config[i].t_step - vec_diffpathbackward_config[i].t_range) + "-" 
				+ boost::lexical_cast<std::string>(vec_diffpathbackward_config[i].t_step)
				);
		}
	}
}
void DiffPathBackwardChainOptions::load_all_changes()
{
	for (unsigned i = 0; i < vec_diffpathbackward_options.size(); ++i)
		vec_diffpathbackward_options[i]->load_all_changes();
}
void DiffPathBackwardChainOptions::add_page()
{
	clear_notebook();
	vec_diffpathbackward_config.push_back( diffpathbackward_config_type() );
	if (vec_diffpathbackward_config.size() > 1) {
		int i = vec_diffpathbackward_config.size()-2;
		vec_diffpathbackward_config.back().t_step = vec_diffpathbackward_config[i].t_step - vec_diffpathbackward_config[i].t_range - 1;
	}
	fill_notebook();
}
void DiffPathBackwardChainOptions::delete_page()
{
	int k = notebook.get_current_page();
	clear_notebook();
	vec_diffpathbackward_config.erase( vec_diffpathbackward_config.begin()+k );
	fill_notebook();
}
void DiffPathBackwardChainOptions::clear_notebook()
{
	notebook.pages().clear();
	for (unsigned i = 0; i < vec_diffpathbackward_options.size(); ++i)
		delete vec_diffpathbackward_options[i];
	vec_diffpathbackward_options.clear();
}
void DiffPathBackwardChainOptions::fill_notebook()
{
	for (unsigned i = 0; i < vec_diffpathbackward_config.size(); ++i) {
		vec_diffpathbackward_options.push_back(new DiffPathBackwardOptions(vec_diffpathbackward_config[i], sg_labels, sg_options));
		notebook.append_page(*vec_diffpathbackward_options[i], "");
	}
	for (unsigned i = 0; i < vec_diffpathbackward_config.size(); ++i)
		vec_diffpathbackward_options[i]->set_chainparent(this);
	show_all();
	if (vec_diffpathbackward_options.size())
		notebook.set_current_page(0);
	update_tabs();
}







ProjectConfigurator::~ProjectConfigurator() {}
ProjectConfigurator::ProjectConfigurator(project_data& projectdata, bool show_project_details)
	: project(projectdata)
	, box_main()
	, sizegroup_labels(Gtk::SizeGroup::create(Gtk::SIZE_GROUP_BOTH))
	, sizegroup_options(Gtk::SizeGroup::create(Gtk::SIZE_GROUP_BOTH))
	, frame_project_details("Project details")	
	, frame_birthdaysearch_options(projectdata.birthdaysearch_config, sizegroup_labels, sizegroup_options)
	, frame_diffpathconnect_options(projectdata.diffpathconnect_config, sizegroup_labels, sizegroup_options)
	, frame_diffpathforward_options(projectdata.diffpathforward_config, sizegroup_labels, sizegroup_options)
	, frame_diffpathbackward_options(projectdata.diffpathbackward_config, sizegroup_labels, sizegroup_options)
	, table_project_details(4,2,false)

{
	if (show_project_details)
		set_label("Project configuration");
	else
		set_label("Default project configuration");
	set_border_width(10);
	frame_project_details.set_border_width(10);
	frame_project_details.add(vbox_tables);
	table_project_details.set_spacings(3);
	table_project_details.set_border_width(10);
	table_project_details.set_col_spacings(10);
	table_project_details.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Chosen-prefix file 1", Gtk::ALIGN_LEFT)), 0,1, 0,1, Gtk::FILL, Gtk::FILL);
	table_project_details.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Chosen-prefix file 2", Gtk::ALIGN_LEFT)), 0,1, 1,2, Gtk::FILL, Gtk::FILL);
	table_project_details.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Working directory", Gtk::ALIGN_LEFT)), 0,1, 2,3, Gtk::FILL, Gtk::FILL);
	table_project_details.attach(cfg_filename1, 1,2, 0,1, Gtk::FILL | Gtk::EXPAND, Gtk::FILL);
	table_project_details.attach(cfg_filename2, 1,2, 1,2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL);
	table_project_details.attach(cfg_workdir, 1,2, 2,3, Gtk::FILL | Gtk::EXPAND, Gtk::FILL);

	table_project_configuration.set_spacings(3);
	table_project_configuration.set_border_width(10);
	table_project_configuration.set_col_spacings(10);
	table_project_configuration.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Set maximum # threads", Gtk::ALIGN_LEFT)), 0,1, 0,1, Gtk::FILL, Gtk::FILL);
	table_project_configuration.attach(*manage_sizegroup(sizegroup_labels, new Gtk::Label("Set automatic mode", Gtk::ALIGN_LEFT)), 0,1, 1,2, Gtk::FILL, Gtk::FILL);
	table_project_configuration.attach(add_sizegroup(sizegroup_options, cfg_nr_threads), 1,2, 0,1, Gtk::FILL, Gtk::FILL);
	table_project_configuration.attach(add_sizegroup(sizegroup_options, cfg_mode_automatic), 1,2, 1,2, Gtk::FILL, Gtk::FILL);
	vbox_tables.pack_start(table_project_details, false, true);
	vbox_tables.pack_start(table_project_configuration, false, true);

	cfg_filename1.set_text(project.filename1.string());
	cfg_filename1.set_editable(false);
	cfg_filename2.set_text(project.filename2.string());
	cfg_filename2.set_editable(false);
	cfg_workdir.set_text(project.workdir.string());
	cfg_workdir.set_editable(false);
	cfg_nr_threads.set_numeric();
	cfg_nr_threads.get_adjustment()->configure(project.max_threads, 1, boost::thread::hardware_concurrency(), 1, 1, 0);
	load_changes();
	cfg_nr_threads.signal_value_changed().connect( sigc::mem_fun(*this, &ProjectConfigurator::save_changes) );
	cfg_mode_automatic.signal_toggled().connect( sigc::mem_fun(*this, &ProjectConfigurator::save_changes) );
	box_main.set_border_width(10);
	if (show_project_details)
		box_main.append_page(frame_project_details, "Project details");
	box_main.append_page(frame_birthdaysearch_options, "Birthday search options");
	box_main.append_page(frame_diffpathconnect_options, "Connect options");
	box_main.append_page(frame_diffpathforward_options, "Forward options");
	box_main.append_page(frame_diffpathbackward_options, "Backward options");
	box_main.show_all();
	box_main.set_current_page(0);
	add(box_main);	
}

void ProjectConfigurator::save_changes() {
	project.max_threads = cfg_nr_threads.get_value_as_int();
	project.mode_automatic = cfg_mode_automatic.get_active();
}
void ProjectConfigurator::load_changes() {
	cfg_nr_threads.set_value(project.max_threads);
	cfg_mode_automatic.set_active(project.mode_automatic);
}
void ProjectConfigurator::load_all_changes() {
	load_changes();
	frame_birthdaysearch_options.load_all_changes();
	frame_diffpathconnect_options.load_all_changes();
	frame_diffpathforward_options.load_all_changes();
	frame_diffpathbackward_options.load_all_changes();
}


























double dist[33][8][17] = 
{
	{ //hybridbits=0
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.81019e-11, 8.13427e-10, 1.69077e-08, 1.8244e-07, 1.71093e-06, 1.25342e-05, 7.42248e-05, 0.000357696, 0.00142681, 0.00477973, 0.0136364, 0.0335913 },
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.42101e-20, 8.11909e-10, 1.7978e-08, 4.14422e-07, 6.58899e-06, 7.93736e-05, 0.00070502, 0.00457788, 0.0217074, 0.0753197, 0.193871, 0.380874, 0.595674 },
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.42101e-20, 1.16089e-10, 1.36405e-08, 5.55195e-07, 1.5087e-05, 0.000265742, 0.0029946, 0.0215068, 0.0985347, 0.292034, 0.580493, 0.831802, 0.958287, 0.994264 },
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.42101e-20, 1.6288e-09, 1.42403e-07, 6.91575e-06, 0.000185357, 0.00272611, 0.0226523, 0.1101, 0.32719, 0.632046, 0.871444, 0.974299, 0.997505, 0.999927 },
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.42101e-20, 1.51402e-08, 1.31906e-06, 5.62687e-05, 0.00111943, 0.0113988, 0.0648665, 0.22248, 0.493628, 0.768704, 0.932429, 0.988476, 0.999017, 0.999973 },
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 5.79953e-10, 8.69929e-08, 7.49635e-06, 0.000235393, 0.00328771, 0.0242214, 0.1054, 0.294219, 0.5658, 0.809646, 0.945149, 0.990495, 0.999149, 0.999975 },
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 1.56723e-09, 4.31917e-07, 2.71093e-05, 0.000590068, 0.00611519, 0.0358611, 0.132291, 0.330318, 0.594051, 0.822231, 0.948182, 0.990847, 0.999161, 0.999975 },
		{ 5.42101e-20, 5.42101e-20, 5.42101e-20, 7.37386e-09, 1.52145e-06, 6.45056e-05, 0.00104097, 0.00871739, 0.044107, 0.147547, 0.347074, 0.604766, 0.826028, 0.948873, 0.990899, 0.999162, 0.999975 }
	},
	{ //hybridbits=1
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 1.0842e-19, 1.0842e-19, 5.80279e-11, 1.56675e-09, 2.6983e-08, 3.06736e-07, 2.71083e-06, 1.92606e-05, 0.000110193, 0.000514459, 0.00198558, 0.00643797, 0.0177911, 0.0424727 },
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 1.0842e-19, 5.81928e-11, 9.89277e-10, 3.13659e-08, 6.08697e-07, 9.44003e-06, 0.000108682, 0.000919962, 0.00570698, 0.0258944, 0.0862273, 0.213922, 0.407302, 0.621299 },
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 1.0842e-19, 2.89343e-10, 2.12956e-08, 7.72372e-07, 1.98403e-05, 0.000330035, 0.00352869, 0.0241957, 0.106663, 0.306513, 0.595489, 0.840812, 0.961345, 0.994818 },
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 1.0842e-19, 3.07679e-09, 1.98308e-07, 8.77257e-06, 0.000218571, 0.00304888, 0.0243476, 0.114949, 0.334804, 0.638565, 0.874394, 0.974979, 0.99758, 0.999927 },
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 1.16031e-10, 1.83328e-08, 1.58156e-06, 6.43692e-05, 0.00122029, 0.0120048, 0.0668071, 0.225914, 0.497028, 0.770591, 0.933005, 0.988564, 0.999024, 0.999973 },
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 2.32421e-10, 1.02323e-07, 8.40179e-06, 0.000252058, 0.00342776, 0.0248055, 0.106745, 0.295964, 0.567081, 0.810178, 0.945271, 0.990509, 0.999149, 0.999975 },
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 1.79896e-09, 5.02142e-07, 2.89934e-05, 0.00061435, 0.0062567, 0.0363064, 0.133085, 0.331169, 0.594584, 0.822417, 0.948215, 0.99085, 0.999161, 0.999975 },
		{ 1.0842e-19, 1.0842e-19, 1.0842e-19, 9.57377e-09, 1.68841e-06, 6.77118e-05, 0.00106848, 0.00883993, 0.0444046, 0.147966, 0.347416, 0.60492, 0.82607, 0.948877, 0.990899, 0.999161, 0.999975 }
	},
	{ //hybridbits=2
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 2.1684e-19, 2.1684e-19, 2.32702e-10, 4.13046e-09, 4.7413e-08, 4.85009e-07, 4.26857e-06, 2.91968e-05, 0.000161859, 0.000730776, 0.00273112, 0.00857668, 0.0229759, 0.05321 },
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 2.1684e-19, 5.79572e-11, 2.26033e-09, 5.0075e-08, 9.86141e-07, 1.43807e-05, 0.000156963, 0.00126092, 0.00742833, 0.0320241, 0.101584, 0.241216, 0.442404, 0.65479 },
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 2.1684e-19, 6.94998e-10, 3.4634e-08, 1.14414e-06, 2.7503e-05, 0.000428982, 0.00430939, 0.0279399, 0.11742, 0.324753, 0.613537, 0.851048, 0.964628, 0.995384 },
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 2.1684e-19, 3.99558e-09, 2.76564e-07, 1.17382e-05, 0.000270023, 0.00352491, 0.0267123, 0.121372, 0.344424, 0.646446, 0.877884, 0.975798, 0.997665, 0.999932 },
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 5.79119e-11, 2.44388e-08, 2.13706e-06, 7.74917e-05, 0.00136815, 0.0128506, 0.0693931, 0.230281, 0.501151, 0.772754, 0.933612, 0.988653, 0.999029, 0.999973 },
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 6.37589e-10, 1.36386e-07, 9.99659e-06, 0.000279307, 0.00363392, 0.025612, 0.108492, 0.298144, 0.568661, 0.810823, 0.945415, 0.990525, 0.99915, 0.999975 },
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 2.89844e-09, 5.92094e-07, 3.19667e-05, 0.000649299, 0.00645268, 0.0368916, 0.1341, 0.332176, 0.595157, 0.822595, 0.948241, 0.99085, 0.999161, 0.999975 },
		{ 2.1684e-19, 2.1684e-19, 2.1684e-19, 1.3041e-08, 1.9234e-06, 7.21573e-05, 0.00110577, 0.00899839, 0.0447859, 0.148508, 0.347876, 0.605145, 0.826126, 0.948884, 0.990898, 0.999162, 0.999975 }
	},
	{ //hybridbits=3
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 4.33681e-19, 4.33681e-19, 2.32824e-10, 6.34447e-09, 7.48531e-08, 7.92185e-07, 6.63096e-06, 4.41691e-05, 0.000236597, 0.00103386, 0.00373919, 0.0113731, 0.0295195, 0.066267 },
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 4.33681e-19, 5.79393e-11, 2.78109e-09, 7.71752e-08, 1.50619e-06, 2.14006e-05, 0.000223366, 0.00170738, 0.00955795, 0.0392028, 0.118692, 0.270283, 0.478193, 0.687467 },
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 4.33681e-19, 5.80387e-10, 4.73596e-08, 1.73942e-06, 3.9075e-05, 0.000567354, 0.00533904, 0.0325891, 0.130071, 0.34531, 0.633246, 0.862116, 0.96822, 0.996011 },
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 4.33681e-19, 6.3355e-09, 4.35696e-07, 1.61197e-05, 0.000340581, 0.00413253, 0.0295469, 0.128688, 0.354913, 0.654739, 0.881448, 0.97658, 0.997746, 0.999933 },
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 1.15673e-10, 4.00229e-08, 2.90172e-06, 9.53643e-05, 0.00155817, 0.0138754, 0.072373, 0.235119, 0.505587, 0.775036, 0.934264, 0.988748, 0.999035, 0.999973 },
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 9.84688e-10, 1.87496e-07, 1.22498e-05, 0.000315269, 0.00388944, 0.0265622, 0.110471, 0.300528, 0.570313, 0.811478, 0.945555, 0.990539, 0.99915, 0.999975 },
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 4.57832e-09, 7.32531e-07, 3.59861e-05, 0.000693774, 0.00668539, 0.0375602, 0.135207, 0.333264, 0.595782, 0.8228, 0.948277, 0.990855, 0.999161, 0.999975 },
		{ 4.33681e-19, 4.33681e-19, 4.33681e-19, 1.85381e-08, 2.2146e-06, 7.75171e-05, 0.00115018, 0.00918173, 0.0452149, 0.149097, 0.348349, 0.605356, 0.826175, 0.948889, 0.990899, 0.999162, 0.999975 }
	},
	{ //hybridbits=4
		{ 8.67362e-19, 8.67362e-19, 8.67362e-19, 8.67362e-19, 8.67362e-19, 1.74182e-10, 6.56085e-09, 1.17399e-07, 1.28053e-06, 1.03678e-05, 6.66003e-05, 0.000344124, 0.00145347, 0.00508586, 0.0149768, 0.037653, 0.0819154 },
		{ 8.67362e-19, 8.67362e-19, 8.67362e-19, 8.67362e-19, 1.74112e-10, 5.39748e-09, 1.29075e-07, 2.38812e-06, 3.18584e-05, 0.000315777, 0.00229539, 0.0122085, 0.0476663, 0.137809, 0.301091, 0.514239, 0.718813 },
		{ 8.67362e-19, 8.67362e-19, 8.67362e-19, 8.67362e-19, 1.45206e-09, 7.67847e-08, 2.58948e-06, 5.4686e-05, 0.000742745, 0.00654533, 0.0377058, 0.143333, 0.366071, 0.652541, 0.872577, 0.97142, 0.996535 },
		{ 8.67362e-19, 8.67362e-19, 8.67362e-19, 1.1599e-10, 9.91712e-09, 6.37769e-07, 2.22549e-05, 0.000431628, 0.00485893, 0.0327378, 0.136573, 0.365899, 0.663311, 0.8851, 0.977415, 0.997833, 0.999936 },
		{ 8.67362e-19, 8.67362e-19, 8.67362e-19, 4.04752e-10, 6.18114e-08, 3.98472e-06, 0.000118167, 0.00178338, 0.0150222, 0.0755731, 0.240154, 0.51011, 0.777329, 0.934903, 0.98884, 0.99904, 0.999973 },
		{ 8.67362e-19, 8.67362e-19, 8.67362e-19, 1.96745e-09, 2.70987e-07, 1.53076e-05, 0.000359233, 0.00418249, 0.0276014, 0.112568, 0.30299, 0.572014, 0.812149, 0.945704, 0.990556, 0.999151, 0.999975 },
		{ 8.67362e-19, 8.67362e-19, 8.67362e-19, 7.12757e-09, 9.29713e-07, 4.12392e-05, 0.000746845, 0.00694954, 0.0382848, 0.13637, 0.334384, 0.596406, 0.822991, 0.948309, 0.990856, 0.999161, 0.999975 },
		{ 8.67362e-19, 8.67362e-19, 5.79422e-11, 2.59581e-08, 2.60607e-06, 8.43302e-05, 0.00120138, 0.00938274, 0.0456695, 0.149705, 0.348828, 0.605583, 0.826234, 0.948896, 0.9909, 0.999162, 0.999975 }
	},
	{ //hybridbits=5
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 1.73472e-18, 1.73472e-18, 6.97856e-10, 1.49457e-08, 2.04762e-07, 2.02843e-06, 1.59565e-05, 9.93757e-05, 0.000497515, 0.00203186, 0.00687846, 0.0196004, 0.0477051, 0.100546 },
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 1.73472e-18, 2.32453e-10, 7.49661e-09, 2.02002e-07, 3.65044e-06, 4.70336e-05, 0.000443121, 0.00305851, 0.0154666, 0.0575113, 0.15891, 0.333442, 0.550356, 0.748816 },
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 1.73472e-18, 3.08284e-09, 1.21801e-07, 3.86641e-06, 7.59163e-05, 0.000961995, 0.00795815, 0.0433602, 0.157287, 0.386956, 0.67121, 0.882368, 0.974387, 0.997011 },
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 5.78627e-11, 1.69538e-08, 9.51783e-07, 3.02424e-05, 0.000540229, 0.00566185, 0.0360945, 0.144586, 0.376847, 0.671718, 0.88865, 0.978192, 0.997911, 0.999938 },
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 6.39095e-10, 9.62128e-08, 5.43817e-06, 0.000145629, 0.00203839, 0.0162582, 0.0789091, 0.245315, 0.514694, 0.779648, 0.935551, 0.988933, 0.999045, 0.999974 },
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 3.00739e-09, 3.73321e-07, 1.91328e-05, 0.000409418, 0.00450093, 0.0286945, 0.11472, 0.305491, 0.573718, 0.812823, 0.945845, 0.99057, 0.999151, 0.999975 },
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 1.14842e-08, 1.2048e-06, 4.7688e-05, 0.000806729, 0.00723232, 0.0390403, 0.137566, 0.335511, 0.597033, 0.823195, 0.948344, 0.990859, 0.999161, 0.999975 },
		{ 1.73472e-18, 1.73472e-18, 1.73472e-18, 3.59781e-08, 3.0279e-06, 9.21161e-05, 0.001256, 0.00959213, 0.0461329, 0.150323, 0.349319, 0.605798, 0.826287, 0.948903, 0.9909, 0.999162, 0.999975 }
	},
	{ //hybridbits=6
		{ 3.46945e-18, 3.46945e-18, 3.46945e-18, 3.46945e-18, 2.32227e-10, 2.49644e-09, 2.98993e-08, 3.36962e-07, 3.25089e-06, 2.45906e-05, 0.000147971, 0.000715618, 0.00282359, 0.00924212, 0.0254734, 0.0600104, 0.122482 },
		{ 3.46945e-18, 3.46945e-18, 3.46945e-18, 3.46945e-18, 4.63532e-10, 1.31527e-08, 3.26153e-07, 5.59153e-06, 6.87358e-05, 0.000616427, 0.00404124, 0.0194364, 0.0688768, 0.182035, 0.367171, 0.586215, 0.777211 },
		{ 3.46945e-18, 3.46945e-18, 3.46945e-18, 3.46945e-18, 4.57782e-09, 1.89835e-07, 5.65725e-06, 0.000103886, 0.00123266, 0.00959538, 0.049558, 0.171846, 0.407901, 0.689313, 0.891592, 0.977055, 0.997422 },
		{ 3.46945e-18, 3.46945e-18, 3.46945e-18, 4.65322e-10, 2.59417e-08, 1.43534e-06, 4.06923e-05, 0.000669468, 0.00655214, 0.0396229, 0.152728, 0.387696, 0.679928, 0.892087, 0.978958, 0.997992, 0.999941 },
		{ 3.46945e-18, 3.46945e-18, 3.46945e-18, 1.39181e-09, 1.41385e-07, 7.33015e-06, 0.000178381, 0.00231431, 0.0175371, 0.0822753, 0.250452, 0.519233, 0.781935, 0.936189, 0.989024, 0.999051, 0.999974 },
		{ 3.46945e-18, 3.46945e-18, 5.81733e-11, 4.47934e-09, 5.26584e-07, 2.37692e-05, 0.000465864, 0.00484038, 0.029821, 0.116901, 0.308001, 0.575425, 0.813499, 0.945993, 0.990587, 0.999152, 0.999974 },
		{ 3.46945e-18, 3.46945e-18, 5.79233e-11, 1.58131e-08, 1.57157e-06, 5.52058e-05, 0.000872278, 0.00752975, 0.0398146, 0.138765, 0.336633, 0.597656, 0.823386, 0.948371, 0.99086, 0.999162, 0.999975 },
		{ 3.46945e-18, 3.46945e-18, 1.15867e-10, 4.87799e-08, 3.68989e-06, 0.000100896, 0.00131598, 0.00981199, 0.0466077, 0.150946, 0.349807, 0.606019, 0.826343, 0.948909, 0.990901, 0.999162, 0.999975 }
	},
	{ //hybridbits=7
		{ 6.93889e-18, 6.93889e-18, 6.93889e-18, 6.93889e-18, 1.74057e-10, 2.78491e-09, 4.65892e-08, 5.33019e-07, 5.12614e-06, 3.75647e-05, 0.000218755, 0.0010222, 0.00389976, 0.0123366, 0.0328824, 0.074945, 0.14807 },
		{ 6.93889e-18, 6.93889e-18, 6.93889e-18, 6.93889e-18, 9.27311e-10, 2.64284e-08, 5.27408e-07, 8.53091e-06, 9.96898e-05, 0.000849573, 0.00529642, 0.0242318, 0.0818971, 0.207186, 0.402052, 0.621551, 0.803887 },
		{ 6.93889e-18, 6.93889e-18, 6.93889e-18, 1.1617e-10, 7.08636e-09, 2.98789e-07, 8.23447e-06, 0.000140649, 0.00156467, 0.0114818, 0.0563143, 0.187003, 0.428854, 0.706805, 0.900209, 0.979489, 0.997782 },
		{ 6.93889e-18, 6.93889e-18, 6.93889e-18, 3.4801e-10, 3.87451e-08, 2.01921e-06, 5.39523e-05, 0.000820296, 0.00753063, 0.0433225, 0.160964, 0.398434, 0.687966, 0.89543, 0.979699, 0.998065, 0.999943 },
		{ 6.93889e-18, 6.93889e-18, 5.7988e-11, 1.56568e-09, 2.14266e-07, 9.72169e-06, 0.000215036, 0.00261086, 0.0188572, 0.085675, 0.255569, 0.523729, 0.78419, 0.936817, 0.989117, 0.999056, 0.999974 },
		{ 6.93889e-18, 6.93889e-18, 5.81263e-11, 5.63825e-09, 7.33263e-07, 2.91668e-05, 0.000525484, 0.00518657, 0.0309545, 0.119079, 0.310499, 0.577123, 0.814156, 0.946132, 0.990599, 0.999152, 0.999975 },
		{ 6.93889e-18, 6.93889e-18, 1.15833e-10, 2.10238e-08, 2.01898e-06, 6.34761e-05, 0.000940832, 0.00783309, 0.040592, 0.139976, 0.337771, 0.598288, 0.823581, 0.948406, 0.990863, 0.999162, 0.999975 },
		{ 6.93889e-18, 6.93889e-18, 1.7386e-10, 6.58351e-08, 4.35694e-06, 0.000110522, 0.00137786, 0.0100349, 0.0470867, 0.151571, 0.350297, 0.606245, 0.826392, 0.948914, 0.9909, 0.999162, 0.999975 }
	},
	{ //hybridbits=8
		{ 1.38778e-17, 1.38778e-17, 1.38778e-17, 1.38778e-17, 5.81628e-11, 4.88568e-09, 7.52627e-08, 8.89601e-07, 8.12442e-06, 5.74081e-05, 0.000322225, 0.0014538, 0.00535383, 0.0163617, 0.0421499, 0.0928932, 0.177595 },
		{ 1.38778e-17, 1.38778e-17, 1.38778e-17, 1.38778e-17, 1.50765e-09, 3.8967e-08, 8.48113e-07, 1.2966e-05, 0.00014364, 0.00116173, 0.00688211, 0.0299706, 0.0966803, 0.23434, 0.437933, 0.656174, 0.828774 },
		{ 1.38778e-17, 1.38778e-17, 1.38778e-17, 1.7444e-10, 1.28504e-08, 4.76977e-07, 1.18265e-05, 0.000188983, 0.00196929, 0.0136365, 0.0636442, 0.202721, 0.449771, 0.723695, 0.908298, 0.981705, 0.998101 },
		{ 1.38778e-17, 1.38778e-17, 1.38778e-17, 7.52886e-10, 6.52694e-08, 2.90382e-06, 7.06514e-05, 0.00099529, 0.00859638, 0.0471873, 0.169336, 0.409138, 0.695832, 0.898657, 0.980392, 0.998138, 0.999945 },
		{ 1.38778e-17, 1.38778e-17, 1.38778e-17, 4.1802e-09, 3.09103e-07, 1.26917e-05, 0.000257181, 0.00292668, 0.0202129, 0.089088, 0.260659, 0.528187, 0.786444, 0.937453, 0.989208, 0.999062, 0.999974 },
		{ 1.38778e-17, 1.38778e-17, 5.78947e-11, 1.18684e-08, 1.00615e-06, 3.52561e-05, 0.000589162, 0.00554162, 0.0320924, 0.121257, 0.312992, 0.578831, 0.814836, 0.946286, 0.990619, 0.999153, 0.999975 },
		{ 1.38778e-17, 1.38778e-17, 1.16267e-10, 3.24385e-08, 2.55137e-06, 7.23854e-05, 0.00101148, 0.00813672, 0.0413671, 0.141182, 0.338904, 0.598922, 0.823787, 0.948443, 0.990867, 0.999162, 0.999975 },
		{ 1.38778e-17, 1.38778e-17, 4.63677e-10, 8.607e-08, 5.16773e-06, 0.00012091, 0.00144193, 0.0102592, 0.0475631, 0.152189, 0.350782, 0.606458, 0.826447, 0.948921, 0.9909, 0.999162, 0.999975 }
	},
	{ //hybridbits=9
		{ 2.77556e-17, 2.77556e-17, 2.77556e-17, 2.77556e-17, 1.73841e-10, 8.05464e-09, 1.26498e-07, 1.4525e-06, 1.26967e-05, 8.70341e-05, 0.000471801, 0.00205246, 0.00729964, 0.0215475, 0.0536299, 0.114251, 0.211274 },
		{ 2.77556e-17, 2.77556e-17, 2.77556e-17, 1.16075e-10, 2.61168e-09, 6.53501e-08, 1.34606e-06, 1.9428e-05, 0.000204991, 0.00157543, 0.00886987, 0.036794, 0.113355, 0.263424, 0.474449, 0.689724, 0.851737 },
		{ 2.77556e-17, 2.77556e-17, 2.77556e-17, 2.32737e-10, 2.03645e-08, 7.21486e-07, 1.68235e-05, 0.000250573, 0.00245456, 0.0160866, 0.0715552, 0.218967, 0.470572, 0.739938, 0.915835, 0.9837, 0.998375 },
		{ 2.77556e-17, 2.77556e-17, 2.77556e-17, 1.21985e-09, 1.02874e-07, 4.08591e-06, 9.10021e-05, 0.00119689, 0.00975831, 0.0512212, 0.1778, 0.419709, 0.703512, 0.901782, 0.981076, 0.998203, 0.999947 },
		{ 2.77556e-17, 2.77556e-17, 1.15743e-10, 5.72929e-09, 4.43586e-07, 1.63124e-05, 0.000304262, 0.00326411, 0.0216114, 0.0925294, 0.265729, 0.532601, 0.788663, 0.938075, 0.9893, 0.999067, 0.999974 },
		{ 2.77556e-17, 2.77556e-17, 1.16178e-10, 1.60906e-08, 1.35446e-06, 4.21569e-05, 0.000657012, 0.00590768, 0.0332432, 0.123421, 0.315461, 0.580505, 0.815491, 0.946425, 0.990631, 0.999153, 0.999975 },
		{ 2.77556e-17, 2.77556e-17, 1.1576e-10, 4.34098e-08, 3.13245e-06, 8.1952e-05, 0.0010832, 0.00844344, 0.0421454, 0.142376, 0.340026, 0.599542, 0.823976, 0.948472, 0.990866, 0.999162, 0.999975 },
		{ 2.77556e-17, 2.77556e-17, 3.47616e-10, 1.16336e-07, 6.05449e-06, 0.000131339, 0.00150591, 0.0104844, 0.0480419, 0.152818, 0.351274, 0.606676, 0.826496, 0.948925, 0.9909, 0.999161, 0.999975 }
	},
	{ //hybridbits=10
		{ 5.55112e-17, 5.55112e-17, 5.55112e-17, 5.55112e-17, 1.04671e-09, 1.58751e-08, 2.119e-07, 2.35835e-06, 1.99184e-05, 0.000131421, 0.000686185, 0.00288216, 0.00989139, 0.0281822, 0.0677354, 0.139411, 0.249259 },
		{ 5.55112e-17, 5.55112e-17, 5.55112e-17, 1.16264e-10, 3.89483e-09, 1.08009e-07, 2.09885e-06, 2.90259e-05, 0.000290183, 0.00211811, 0.0113428, 0.0448353, 0.132001, 0.294334, 0.511396, 0.722025, 0.872775 },
		{ 5.55112e-17, 5.55112e-17, 5.55112e-17, 4.6297e-10, 3.11347e-08, 1.08665e-06, 2.36509e-05, 0.000329496, 0.00303483, 0.0188506, 0.0800677, 0.235715, 0.491254, 0.755579, 0.922871, 0.985516, 0.998615 },
		{ 5.55112e-17, 5.55112e-17, 5.78901e-11, 1.62092e-09, 1.58098e-07, 5.67028e-06, 0.000116485, 0.0014282, 0.0110173, 0.055419, 0.186349, 0.430195, 0.711042, 0.904844, 0.98174, 0.998274, 0.999949 },
		{ 5.55112e-17, 5.55112e-17, 5.8105e-11, 8.42523e-09, 6.32996e-07, 2.07452e-05, 0.000357952, 0.0036259, 0.0230545, 0.0960078, 0.270789, 0.536987, 0.790871, 0.938695, 0.989392, 0.999073, 0.999974 },
		{ 5.55112e-17, 5.55112e-17, 1.16143e-10, 2.55515e-08, 1.77496e-06, 4.99517e-05, 0.000729701, 0.0062842, 0.0344041, 0.1256, 0.31795, 0.582201, 0.816165, 0.946575, 0.990649, 0.999154, 0.999975 },
		{ 5.55112e-17, 5.55112e-17, 2.3138e-10, 6.10844e-08, 3.84329e-06, 9.20619e-05, 0.00115807, 0.00875532, 0.0429246, 0.143579, 0.341167, 0.600184, 0.824185, 0.948509, 0.990871, 0.999162, 0.999975 },
		{ 5.55112e-17, 5.55112e-17, 8.68358e-10, 1.47968e-07, 6.98646e-06, 0.000142409, 0.00157195, 0.0107114, 0.0485237, 0.153446, 0.351777, 0.606915, 0.826563, 0.948939, 0.990902, 0.999162, 0.999975 }
	},
	{ //hybridbits=11
		{ 1.11022e-16, 1.11022e-16, 1.11022e-16, 1.11022e-16, 9.29787e-10, 2.61503e-08, 3.52447e-07, 3.79307e-06, 3.09492e-05, 0.000196955, 0.00099259, 0.00402172, 0.013313, 0.0365947, 0.0848868, 0.168719, 0.291549 },
		{ 1.11022e-16, 1.11022e-16, 1.11022e-16, 2.32427e-10, 6.91471e-09, 1.76877e-07, 3.26165e-06, 4.29118e-05, 0.000407332, 0.00282432, 0.0143929, 0.0542351, 0.152686, 0.326914, 0.548424, 0.752825, 0.891827 },
		{ 1.11022e-16, 1.11022e-16, 1.11022e-16, 9.84595e-10, 5.11989e-08, 1.63194e-06, 3.28189e-05, 0.000428654, 0.00372282, 0.0219528, 0.0891708, 0.252901, 0.511715, 0.770548, 0.92941, 0.987128, 0.998822 },
		{ 1.11022e-16, 1.11022e-16, 1.16265e-10, 3.48795e-09, 2.29972e-07, 7.78842e-06, 0.000147378, 0.00168984, 0.0123789, 0.0597802, 0.194972, 0.440542, 0.718352, 0.907778, 0.982369, 0.998335, 0.999952 },
		{ 1.11022e-16, 1.11022e-16, 5.80356e-11, 1.26518e-08, 8.73552e-07, 2.60645e-05, 0.000417452, 0.00400865, 0.0245357, 0.0995051, 0.275808, 0.54129, 0.793009, 0.939287, 0.989474, 0.999078, 0.999974 },
		{ 1.11022e-16, 1.11022e-16, 3.47591e-10, 3.64391e-08, 2.27892e-06, 5.85271e-05, 0.000805376, 0.00666826, 0.0355726, 0.127758, 0.32038, 0.583843, 0.816799, 0.946703, 0.99066, 0.999154, 0.999975 },
		{ 1.11022e-16, 1.11022e-16, 2.31405e-10, 8.84546e-08, 4.66275e-06, 0.000102924, 0.0012346, 0.0090683, 0.043705, 0.144764, 0.342259, 0.600773, 0.824356, 0.948531, 0.990871, 0.999161, 0.999975 },
		{ 1.11022e-16, 1.11022e-16, 1.56386e-09, 1.93687e-07, 8.0976e-06, 0.000153876, 0.00163794, 0.0109378, 0.0490011, 0.154058, 0.352232, 0.607098, 0.826594, 0.948932, 0.990898, 0.999162, 0.999974 }
	},
	{ //hybridbits=12
		{ 2.22045e-16, 2.22045e-16, 2.22045e-16, 1.16004e-10, 2.49408e-09, 4.43134e-08, 5.97941e-07, 6.0942e-06, 4.78506e-05, 0.000293586, 0.00142698, 0.00557415, 0.0177911, 0.0471671, 0.105544, 0.202474, 0.338013 },
		{ 2.22045e-16, 2.22045e-16, 2.22045e-16, 3.48936e-10, 1.03518e-08, 2.85371e-07, 5.04154e-06, 6.29477e-05, 0.000567051, 0.00373527, 0.0181157, 0.0651275, 0.175467, 0.361044, 0.585328, 0.782027, 0.909 },
		{ 2.22045e-16, 2.22045e-16, 2.22045e-16, 1.27531e-09, 7.91275e-08, 2.42043e-06, 4.52443e-05, 0.000553099, 0.00453378, 0.0254213, 0.0988911, 0.27054, 0.532008, 0.78495, 0.93552, 0.988622, 0.999002 },
		{ 2.22045e-16, 2.22045e-16, 2.22045e-16, 6.32079e-09, 3.44686e-07, 1.05401e-05, 0.000184432, 0.0019868, 0.0138425, 0.0643116, 0.203718, 0.450878, 0.725615, 0.910683, 0.983003, 0.998401, 0.999952 },
		{ 2.22045e-16, 2.22045e-16, 1.73781e-10, 1.92897e-08, 1.19683e-06, 3.23271e-05, 0.000483338, 0.00441509, 0.0260619, 0.103042, 0.280848, 0.54563, 0.795213, 0.939926, 0.989574, 0.999083, 0.999974 },
		{ 2.22045e-16, 2.22045e-16, 5.79821e-10, 5.40973e-08, 2.86942e-06, 6.82529e-05, 0.00088592, 0.00706382, 0.0367511, 0.129945, 0.322877, 0.585575, 0.817508, 0.946878, 0.990684, 0.999155, 0.999975 },
		{ 2.22045e-16, 2.22045e-16, 8.71224e-10, 1.22959e-07, 5.60052e-06, 0.000114402, 0.00131347, 0.00938681, 0.0444933, 0.145984, 0.343431, 0.60146, 0.824605, 0.94859, 0.990879, 0.999163, 0.999975 },
		{ 2.22045e-16, 2.22045e-16, 2.38077e-09, 2.48761e-07, 9.29909e-06, 0.000165588, 0.00170499, 0.0111656, 0.0494838, 0.154702, 0.352772, 0.607381, 0.826696, 0.948964, 0.990905, 0.999162, 0.999975 }
	},
	{ //hybridbits=13
		{ 4.44089e-16, 4.44089e-16, 4.44089e-16, 1.15884e-10, 3.59239e-09, 7.43393e-08, 9.8472e-07, 9.71724e-06, 7.3753e-05, 0.000436011, 0.00204008, 0.00767661, 0.023608, 0.0603197, 0.130135, 0.240847, 0.388321 },
		{ 4.44089e-16, 4.44089e-16, 4.44089e-16, 4.63471e-10, 1.94078e-08, 4.56403e-07, 7.74002e-06, 9.13234e-05, 0.000781968, 0.00490072, 0.0226283, 0.0776484, 0.200321, 0.396404, 0.62165, 0.809297, 0.924173 },
		{ 4.44089e-16, 4.44089e-16, 5.81314e-11, 2.9647e-09, 1.20564e-07, 3.49689e-06, 6.14768e-05, 0.000706356, 0.00547735, 0.0292581, 0.109174, 0.288459, 0.551877, 0.798597, 0.941106, 0.989901, 0.999157 },
		{ 4.44089e-16, 4.44089e-16, 1.15663e-10, 9.02168e-09, 4.98043e-07, 1.40332e-05, 0.000228367, 0.00231905, 0.015408, 0.0689726, 0.212445, 0.460925, 0.732503, 0.913375, 0.983556, 0.998455, 0.999955 },
		{ 4.44089e-16, 4.44089e-16, 1.15814e-10, 3.21963e-08, 1.60657e-06, 3.95616e-05, 0.000555177, 0.00484168, 0.0276113, 0.106545, 0.285736, 0.549747, 0.797222, 0.940454, 0.98964, 0.999088, 0.999974 },
		{ 4.44089e-16, 4.44089e-16, 6.94052e-10, 7.77917e-08, 3.64429e-06, 7.8939e-05, 0.000970894, 0.00746537, 0.0379234, 0.132058, 0.325217, 0.587104, 0.818057, 0.946963, 0.990683, 0.999154, 0.999975 },
		{ 4.44089e-16, 4.44089e-16, 1.27919e-09, 1.67691e-07, 6.66837e-06, 0.00012695, 0.00139446, 0.0097019, 0.0452551, 0.147126, 0.344455, 0.601963, 0.824693, 0.948564, 0.990867, 0.99916, 0.999975 },
		{ 4.44089e-16, 4.44089e-16, 3.00668e-09, 3.2079e-07, 1.05582e-05, 0.000177942, 0.00177213, 0.0113874, 0.0499384, 0.155263, 0.353153, 0.607467, 0.826643, 0.948918, 0.990893, 0.999161, 0.999975 }
	},
	{ //hybridbits=14
		{ 8.88178e-16, 8.88178e-16, 8.88178e-16, 3.48962e-10, 7.09557e-09, 1.28302e-07, 1.61349e-06, 1.53955e-05, 0.000112735, 0.000642674, 0.00289657, 0.0104999, 0.0311008, 0.0765391, 0.159112, 0.283946, 0.441992 },
		{ 8.88178e-16, 8.88178e-16, 8.88178e-16, 1.0999e-09, 3.17233e-08, 7.43819e-07, 1.17508e-05, 0.000131521, 0.00107035, 0.00637642, 0.0280548, 0.0919567, 0.227349, 0.432998, 0.657428, 0.83487, 0.937689 },
		{ 8.88178e-16, 8.88178e-16, 1.15658e-10, 3.75888e-09, 1.95173e-07, 5.04615e-06, 8.27667e-05, 0.000894673, 0.00657571, 0.0335191, 0.120137, 0.306918, 0.571747, 0.811873, 0.946465, 0.991143, 0.999289 },
		{ 8.88178e-16, 8.88178e-16, 2.32312e-10, 1.6378e-08, 7.25453e-07, 1.85629e-05, 0.000280237, 0.00268985, 0.0170865, 0.0738398, 0.221408, 0.471193, 0.739601, 0.916233, 0.9842, 0.998519, 0.999956 },
		{ 8.88178e-16, 8.88178e-16, 5.783e-11, 4.59748e-08, 2.13757e-06, 4.82946e-05, 0.000635979, 0.00529766, 0.0292354, 0.110193, 0.29088, 0.554215, 0.799557, 0.941181, 0.989769, 0.999096, 0.999974 },
		{ 8.88178e-16, 8.88178e-16, 7.52217e-10, 1.11212e-07, 4.55792e-06, 9.09778e-05, 0.00106213, 0.00788821, 0.0391503, 0.134317, 0.32784, 0.589017, 0.818934, 0.947221, 0.99073, 0.999158, 0.999975 },
		{ 8.88178e-16, 8.88178e-16, 2.09359e-09, 2.21106e-07, 7.89557e-06, 0.000140516, 0.00147944, 0.0100349, 0.0460756, 0.148422, 0.345769, 0.602839, 0.825107, 0.948707, 0.990899, 0.999164, 0.999975 },
		{ 8.88178e-16, 8.88178e-16, 4.62612e-09, 3.79804e-07, 1.18861e-05, 0.000190439, 0.00184098, 0.011625, 0.0504576, 0.155998, 0.353851, 0.607951, 0.826916, 0.949032, 0.990922, 0.999164, 0.999975 }
	},
	{ //hybridbits=15
		{ 1.77636e-15, 1.77636e-15, 1.77636e-15, 8.1173e-10, 1.35675e-08, 2.13485e-07, 2.65604e-06, 2.43163e-05, 0.00017128, 0.000940942, 0.00408589, 0.0142564, 0.0406444, 0.0962887, 0.192764, 0.331555, 0.498221 },
		{ 1.77636e-15, 1.77636e-15, 5.78987e-11, 1.33167e-09, 5.27457e-08, 1.18414e-06, 1.7636e-05, 0.000188077, 0.00145253, 0.00822898, 0.0345056, 0.108064, 0.25618, 0.470027, 0.691715, 0.857979, 0.949181 },
		{ 1.77636e-15, 1.77636e-15, 1.77636e-15, 7.15744e-09, 3.00205e-07, 7.19596e-06, 0.000110376, 0.00112336, 0.00783132, 0.0381383, 0.131465, 0.325165, 0.590607, 0.823974, 0.95105, 0.992115, 0.999403 },
		{ 1.77636e-15, 1.77636e-15, 2.31666e-10, 2.42091e-08, 1.02883e-06, 2.41961e-05, 0.000341405, 0.00309958, 0.0188531, 0.0787383, 0.230059, 0.480689, 0.745827, 0.918515, 0.984629, 0.998563, 0.999958 },
		{ 1.77636e-15, 1.77636e-15, 9.30605e-10, 7.18311e-08, 2.82148e-06, 5.84496e-05, 0.000724348, 0.0057704, 0.0308398, 0.113643, 0.29549, 0.557889, 0.801172, 0.941506, 0.989781, 0.999095, 0.999974 },
		{ 1.77636e-15, 1.77636e-15, 1.33e-09, 1.51736e-07, 5.61134e-06, 0.000104106, 0.00115643, 0.00830227, 0.0402937, 0.136285, 0.32987, 0.590143, 0.819145, 0.947137, 0.990684, 0.999152, 0.999974 },
		{ 1.77636e-15, 1.77636e-15, 2.55949e-09, 2.80264e-07, 9.05261e-06, 0.000153203, 0.00156001, 0.010337, 0.0467772, 0.149395, 0.346481, 0.602954, 0.824881, 0.948527, 0.990843, 0.999157, 0.999974 },
		{ 1.77636e-15, 1.77636e-15, 5.95841e-09, 4.54054e-07, 1.31422e-05, 0.000202425, 0.00190475, 0.0118313, 0.0508507, 0.1564, 0.353926, 0.607665, 0.826547, 0.948825, 0.990863, 0.999157, 0.999974 }
	},
	{ //hybridbits=16
		{ 3.55271e-15, 3.55271e-15, 5.82057e-11, 9.31291e-10, 2.49702e-08, 3.70945e-07, 4.31508e-06, 3.8267e-05, 0.000259377, 0.00137116, 0.00573032, 0.0192369, 0.0527574, 0.120225, 0.231601, 0.383657, 0.556218 },
		{ 3.55271e-15, 3.55271e-15, 3.55271e-15, 2.38043e-09, 9.27787e-08, 1.87926e-06, 2.63854e-05, 0.000266667, 0.00195581, 0.0105481, 0.0421973, 0.126399, 0.287491, 0.508386, 0.725634, 0.87983, 0.959466 },
		{ 3.55271e-15, 3.55271e-15, 1.16389e-10, 1.15807e-08, 4.57291e-07, 1.01509e-05, 0.00014619, 0.00140312, 0.00929944, 0.0433435, 0.143859, 0.344654, 0.610366, 0.836559, 0.955946, 0.993185, 0.999503 },
		{ 3.55271e-15, 3.55271e-15, 5.22557e-10, 3.91918e-08, 1.46827e-06, 3.15183e-05, 0.000415027, 0.00357256, 0.0208267, 0.0841337, 0.239641, 0.49151, 0.753377, 0.921696, 0.985379, 0.998637, 0.999959 },
		{ 3.55271e-15, 3.55271e-15, 1.04424e-09, 9.60701e-08, 3.62119e-06, 7.01677e-05, 0.000823968, 0.00629795, 0.0326337, 0.117602, 0.301139, 0.56304, 0.804163, 0.942595, 0.990005, 0.999115, 0.999974 },
		{ 3.55271e-15, 3.55271e-15, 2.73342e-09, 1.98086e-07, 6.6706e-06, 0.000117486, 0.00125517, 0.0087535, 0.0416161, 0.138807, 0.333038, 0.592803, 0.820672, 0.947728, 0.99082, 0.999166, 0.999975 },
		{ 3.55271e-15, 3.55271e-15, 4.12815e-09, 3.39438e-07, 1.03807e-05, 0.000167265, 0.00164863, 0.0106932, 0.0477028, 0.150998, 0.348393, 0.604596, 0.825925, 0.948987, 0.990962, 0.999172, 0.999975 },
		{ 3.55271e-15, 3.55271e-15, 7.85642e-09, 5.25623e-07, 1.44769e-05, 0.000215464, 0.00197924, 0.0120985, 0.0514914, 0.157468, 0.355243, 0.608909, 0.827443, 0.949255, 0.990981, 0.999172, 0.999975 }
	},
	{ //hybridbits=17
		{ 7.10543e-15, 7.10543e-15, 5.82028e-11, 1.22226e-09, 4.24298e-08, 6.19045e-07, 7.03369e-06, 5.97189e-05, 0.00039005, 0.00198172, 0.0079622, 0.0256957, 0.0677451, 0.148452, 0.275173, 0.439214, 0.615008 },
		{ 7.10543e-15, 7.10543e-15, 1.15915e-10, 4.98435e-09, 1.54747e-07, 2.91695e-06, 3.90155e-05, 0.000374145, 0.00260516, 0.0133629, 0.0509956, 0.146172, 0.319371, 0.545188, 0.756157, 0.89822, 0.96766 },
		{ 7.10543e-15, 7.10543e-15, 3.47735e-10, 1.9647e-08, 6.81561e-07, 1.41854e-05, 0.000190633, 0.00173003, 0.0109104, 0.0487168, 0.155923, 0.362581, 0.627498, 0.846622, 0.959384, 0.99385, 0.999578 },
		{ 7.10543e-15, 7.10543e-15, 5.22756e-10, 5.48313e-08, 2.00849e-06, 3.9953e-05, 0.000495487, 0.00406159, 0.0227453, 0.0890244, 0.247603, 0.499467, 0.757923, 0.922976, 0.985518, 0.998648, 0.999961 },
		{ 7.10543e-15, 5.80429e-11, 2.0315e-09, 1.33673e-07, 4.47726e-06, 8.17273e-05, 0.000916188, 0.00676037, 0.0340965, 0.120488, 0.304499, 0.565014, 0.804315, 0.942175, 0.989822, 0.99909, 0.999974 },
		{ 7.10543e-15, 7.10543e-15, 3.14054e-09, 2.45951e-07, 7.78034e-06, 0.000130529, 0.00134284, 0.00911621, 0.0425363, 0.140143, 0.333854, 0.592358, 0.819571, 0.946985, 0.990594, 0.999137, 0.999974 },
		{ 7.10543e-15, 5.81992e-11, 6.28552e-09, 3.9721e-07, 1.15575e-05, 0.000179774, 0.00172068, 0.0109387, 0.0481659, 0.15132, 0.34789, 0.603165, 0.824427, 0.948168, 0.990729, 0.999142, 0.999974 },
		{ 7.10543e-15, 1.15697e-10, 1.05863e-08, 6.0475e-07, 1.57873e-05, 0.000226493, 0.00203273, 0.0122383, 0.0516321, 0.157195, 0.354095, 0.607089, 0.825826, 0.948418, 0.990746, 0.999142, 0.999974 }
	},
	{ //hybridbits=18
		{ 1.42109e-14, 1.42109e-14, 1.42109e-14, 3.30298e-09, 6.97682e-08, 1.04027e-06, 1.13236e-05, 9.3086e-05, 0.000585212, 0.00286375, 0.0110621, 0.0342995, 0.0868095, 0.182515, 0.324653, 0.497852, 0.671658 },
		{ 1.42109e-14, 1.42109e-14, 1.16092e-10, 8.7069e-09, 2.48031e-07, 4.5298e-06, 5.7631e-05, 0.000523707, 0.00347176, 0.016983, 0.0618848, 0.169719, 0.35586, 0.585704, 0.788466, 0.916698, 0.975181 },
		{ 1.42109e-14, 1.42109e-14, 5.80705e-10, 3.01386e-08, 1.00503e-06, 1.96166e-05, 0.000248011, 0.00213829, 0.0128831, 0.0552215, 0.170473, 0.384353, 0.648898, 0.860108, 0.964589, 0.994938, 0.99967 },
		{ 1.42109e-14, 1.42109e-14, 1.09959e-09, 8.3222e-08, 2.66739e-06, 4.99728e-05, 0.000588703, 0.00462857, 0.0250366, 0.0952023, 0.258659, 0.512441, 0.767721, 0.927557, 0.986709, 0.998779, 0.999964 },
		{ 1.42109e-14, 1.42109e-14, 2.9524e-09, 1.82354e-07, 5.56816e-06, 9.58449e-05, 0.0010306, 0.0073612, 0.0361881, 0.125367, 0.312142, 0.573054, 0.809933, 0.944652, 0.99043, 0.999157, 0.999976 },
		{ 1.42109e-14, 1.42109e-14, 4.58274e-09, 3.19516e-07, 9.26758e-06, 0.000146912, 0.00146133, 0.00967138, 0.0442681, 0.143839, 0.339347, 0.598097, 0.823682, 0.948878, 0.991092, 0.999198, 0.999976 },
		{ 1.42109e-14, 5.80524e-11, 8.18539e-09, 4.93213e-07, 1.32529e-05, 0.00019696, 0.00183202, 0.0114133, 0.0495564, 0.154197, 0.352214, 0.607876, 0.828, 0.949902, 0.991205, 0.999202, 0.999976 },
		{ 1.42109e-14, 2.32657e-10, 1.19818e-08, 7.11755e-07, 1.75918e-05, 0.0002431, 0.00213191, 0.0126372, 0.0527658, 0.159571, 0.357829, 0.61138, 0.829213, 0.950113, 0.991219, 0.999202, 0.999976 }
	},
	{ //hybridbits=19
		{ 2.84217e-14, 2.84217e-14, 2.32755e-10, 5.64431e-09, 1.19811e-07, 1.72285e-06, 1.80378e-05, 0.000142556, 0.000859756, 0.00403434, 0.0149698, 0.0446264, 0.108706, 0.220253, 0.378002, 0.560117, 0.731733 },
		{ 2.84217e-14, 2.84217e-14, 4.06472e-10, 1.56782e-08, 3.99156e-07, 6.90759e-06, 8.26977e-05, 0.000710807, 0.00447237, 0.0208462, 0.0726513, 0.191318, 0.386953, 0.617851, 0.812598, 0.930101, 0.980715 },
		{ 2.84217e-14, 2.84217e-14, 1.04381e-09, 4.9117e-08, 1.44718e-06, 2.59843e-05, 0.000308771, 0.00252145, 0.0145323, 0.0600379, 0.179829, 0.395996, 0.657525, 0.863446, 0.965125, 0.994994, 0.999689 },
		{ 2.84217e-14, 2.84217e-14, 2.14648e-09, 1.17882e-07, 3.49713e-06, 6.04682e-05, 0.000672194, 0.00505193, 0.0264319, 0.0980691, 0.26189, 0.513393, 0.765782, 0.925325, 0.985841, 0.998659, 0.99996 },
		{ 2.84217e-14, 5.78723e-11, 3.99319e-09, 2.42948e-07, 6.79774e-06, 0.000108944, 0.00111535, 0.00769231, 0.0369255, 0.12593, 0.310748, 0.568674, 0.8047, 0.941498, 0.989489, 0.999037, 0.999971 },
		{ 2.84217e-14, 2.84217e-14, 7.46576e-09, 4.08186e-07, 1.0657e-05, 0.000159577, 0.00152478, 0.00983243, 0.0442853, 0.142594, 0.335247, 0.591406, 0.817449, 0.94555, 0.990145, 0.999078, 0.999972 },
		{ 2.84217e-14, 2.84217e-14, 1.11152e-08, 6.07054e-07, 1.46957e-05, 0.000207492, 0.00186768, 0.0114143, 0.0490287, 0.151842, 0.346775, 0.600291, 0.821469, 0.946538, 0.990258, 0.999081, 0.999972 },
		{ 2.84217e-14, 5.80292e-11, 1.68285e-08, 8.34809e-07, 1.89062e-05, 0.000250058, 0.00213872, 0.0125037, 0.0518628, 0.156586, 0.351768, 0.603456, 0.8226, 0.946747, 0.990272, 0.999081, 0.999972 }
	},
	{ //hybridbits=20
		{ 5.68434e-14, 5.68434e-14, 1.16132e-10, 1.04519e-08, 2.12812e-07, 2.93651e-06, 2.99754e-05, 0.000227913, 0.00132285, 0.00595815, 0.0211286, 0.0599754, 0.138725, 0.266623, 0.434793, 0.615137, 0.773454 },
		{ 5.68434e-14, 5.68434e-14, 5.22575e-10, 2.62449e-08, 6.73716e-07, 1.0941e-05, 0.000124962, 0.00102337, 0.00612364, 0.0271359, 0.08993, 0.225332, 0.434509, 0.664643, 0.844797, 0.945592, 0.985946 },
		{ 5.68434e-14, 5.81992e-11, 1.51318e-09, 8.24683e-08, 2.31388e-06, 3.82873e-05, 0.000424751, 0.00326782, 0.0178534, 0.0703177, 0.20185, 0.428145, 0.688676, 0.882597, 0.972065, 0.996304, 0.999791 },
		{ 5.68434e-14, 5.78927e-11, 4.45774e-09, 1.9863e-07, 5.23565e-06, 8.31804e-05, 0.000862795, 0.00612654, 0.0305818, 0.109128, 0.282198, 0.538697, 0.786482, 0.935783, 0.988766, 0.999029, 0.999973 },
		{ 5.68434e-14, 5.80076e-11, 8.46911e-09, 3.9364e-07, 9.63721e-06, 0.0001408, 0.0013499, 0.00887245, 0.0410313, 0.135948, 0.327895, 0.588945, 0.820628, 0.949272, 0.991591, 0.999293, 0.99998 },
		{ 5.68434e-14, 1.15693e-10, 1.37096e-08, 6.22428e-07, 1.43079e-05, 0.000196766, 0.00177542, 0.0109984, 0.0481123, 0.151604, 0.350448, 0.609364, 0.831648, 0.952565, 0.992079, 0.999321, 0.99998 },
		{ 5.68434e-14, 5.68434e-14, 2.18018e-08, 8.64439e-07, 1.88239e-05, 0.000246395, 0.00211424, 0.0125069, 0.0525398, 0.160105, 0.360869, 0.617191, 0.835041, 0.953342, 0.992158, 0.999323, 0.99998 },
		{ 5.68434e-14, 1.73709e-10, 2.96464e-08, 1.14914e-06, 2.33678e-05, 0.00028909, 0.00237313, 0.0135221, 0.05514, 0.164398, 0.365289, 0.619898, 0.835959, 0.953497, 0.992168, 0.999323, 0.99998 }
	},
	{ //hybridbits=21
		{ 1.13687e-13, 1.13687e-13, 6.39943e-10, 1.76857e-08, 3.60986e-07, 4.83779e-06, 4.80925e-05, 0.000357895, 0.00202802, 0.00890692, 0.0307274, 0.084512, 0.18841, 0.346987, 0.539248, 0.724496, 0.865377 },
		{ 1.13687e-13, 1.13687e-13, 9.85302e-10, 4.0977e-08, 9.65422e-07, 1.48883e-05, 0.000163175, 0.0012861, 0.00744695, 0.0320166, 0.103119, 0.251352, 0.471691, 0.702595, 0.871923, 0.958834, 0.990274 },
		{ 1.13687e-13, 1.13687e-13, 3.14187e-09, 1.27653e-07, 3.15613e-06, 4.9365e-05, 0.000519167, 0.00381179, 0.0199858, 0.0759613, 0.211709, 0.438929, 0.695332, 0.884312, 0.97186, 0.996116, 0.999764 },
		{ 1.13687e-13, 5.80383e-11, 5.39756e-09, 2.71561e-07, 6.6231e-06, 9.85015e-05, 0.000964912, 0.0065366, 0.0314368, 0.10915, 0.277428, 0.52608, 0.770451, 0.92486, 0.984968, 0.99846, 0.99995 },
		{ 1.13687e-13, 1.13687e-13, 1.15077e-08, 5.23834e-07, 1.15505e-05, 0.000157487, 0.00142495, 0.0089624, 0.0401863, 0.130695, 0.313158, 0.565007, 0.797399, 0.936241, 0.987735, 0.998795, 0.999962 },
		{ 1.13687e-13, 1.13687e-13, 1.85905e-08, 7.7197e-07, 1.62816e-05, 0.000208274, 0.00178331, 0.0106476, 0.0455647, 0.142296, 0.329737, 0.58025, 0.806072, 0.939159, 0.988267, 0.998834, 0.999963 },
		{ 1.13687e-13, 1.16343e-10, 2.72825e-08, 1.04721e-06, 2.05389e-05, 0.000250186, 0.0020478, 0.0117693, 0.0487662, 0.14837, 0.337228, 0.586058, 0.808777, 0.939878, 0.988364, 0.998839, 0.999963 },
		{ 1.13687e-13, 2.31362e-10, 3.95629e-08, 1.31067e-06, 2.43092e-05, 0.000282999, 0.0022352, 0.0124782, 0.0505424, 0.151279, 0.340261, 0.587985, 0.809489, 0.940019, 0.988375, 0.998839, 0.999963 }
	},
	{ //hybridbits=22
		{ 2.27374e-13, 2.27374e-13, 1.15831e-09, 2.77995e-08, 5.59812e-07, 7.07427e-06, 6.65946e-05, 0.00047216, 0.00256862, 0.0108607, 0.0362004, 0.0965169, 0.209258, 0.375899, 0.571379, 0.753017, 0.885353 },
		{ 2.27374e-13, 5.78592e-11, 2.14079e-09, 7.37704e-08, 1.58482e-06, 2.35346e-05, 0.000247924, 0.00188322, 0.0104883, 0.0432809, 0.133333, 0.309479, 0.550684, 0.777111, 0.919934, 0.979771, 0.99646 },
		{ 2.27374e-13, 2.27374e-13, 4.93897e-09, 2.02556e-07, 4.61869e-06, 6.92403e-05, 0.000703478, 0.00500517, 0.0254643, 0.0938007, 0.252502, 0.503033, 0.762057, 0.927787, 0.988099, 0.999127, 0.999978 },
		{ 2.27374e-13, 5.796e-11, 1.05487e-08, 4.12791e-07, 9.62397e-06, 0.000137014, 0.00129885, 0.00853543, 0.0398225, 0.133814, 0.327682, 0.595291, 0.83224, 0.958312, 0.994814, 0.999752, 0.999998 },
		{ 2.27374e-13, 5.81039e-11, 1.74312e-08, 7.34027e-07, 1.59125e-05, 0.000211276, 0.00186498, 0.0114404, 0.0499644, 0.15776, 0.365086, 0.632358, 0.854058, 0.965201, 0.995807, 0.999803, 0.999998 },
		{ 2.27374e-13, 1.15749e-10, 2.80112e-08, 1.09087e-06, 2.2081e-05, 0.000276657, 0.00231295, 0.0134905, 0.0562941, 0.170786, 0.38242, 0.646517, 0.86058, 0.966683, 0.995944, 0.999807, 0.999998 },
		{ 2.27374e-13, 5.81699e-10, 4.05444e-08, 1.46361e-06, 2.7634e-05, 0.000328911, 0.00263516, 0.0148173, 0.0599326, 0.177325, 0.389821, 0.651458, 0.862331, 0.966958, 0.99596, 0.999807, 0.999998 },
		{ 2.27374e-13, 5.79523e-10, 5.57501e-08, 1.83946e-06, 3.27103e-05, 0.000370743, 0.00286403, 0.0156519, 0.0619356, 0.180399, 0.392683, 0.652945, 0.862706, 0.966996, 0.99596, 0.999807, 0.999998 }
	},
	{ //hybridbits=23
		{ 4.54747e-13, 4.54747e-13, 1.79928e-09, 5.31077e-08, 9.37713e-07, 1.1757e-05, 0.000107215, 0.000737111, 0.00387193, 0.0157821, 0.050546, 0.128968, 0.266446, 0.454619, 0.655935, 0.823282, 0.929631 },
		{ 4.54747e-13, 4.54747e-13, 2.84484e-09, 1.14374e-07, 2.30217e-06, 3.21455e-05, 0.000318826, 0.00230222, 0.0122718, 0.0487365, 0.145248, 0.327863, 0.570452, 0.791759, 0.927378, 0.982363, 0.997072 },
		{ 4.54747e-13, 5.80326e-11, 7.02194e-09, 3.05019e-07, 6.71135e-06, 9.50486e-05, 0.000920893, 0.00628069, 0.0307078, 0.108897, 0.282475, 0.542915, 0.796018, 0.945008, 0.992736, 0.999673, 0.999998 },
		{ 4.54747e-13, 2.31161e-10, 1.47365e-08, 5.91253e-07, 1.28516e-05, 0.000174768, 0.00159383, 0.0101313, 0.0458531, 0.149679, 0.356275, 0.629463, 0.857934, 0.969477, 0.997235, 0.999947, 1 },
		{ 4.54747e-13, 2.90656e-10, 2.55196e-08, 1.02793e-06, 2.07905e-05, 0.000265048, 0.00226421, 0.013476, 0.0572158, 0.175706, 0.395435, 0.666365, 0.878043, 0.975002, 0.997822, 0.999959, 1 },
		{ 4.54747e-13, 5.78754e-10, 4.08022e-08, 1.46251e-06, 2.8161e-05, 0.000340002, 0.00276214, 0.0156981, 0.0638846, 0.188996, 0.412424, 0.679474, 0.883539, 0.976037, 0.997879, 0.99996, 1 },
		{ 4.54747e-13, 5.20257e-10, 5.8789e-08, 1.91703e-06, 3.48851e-05, 0.000401643, 0.00312921, 0.0171641, 0.0677812, 0.195751, 0.419722, 0.684012, 0.884959, 0.976208, 0.997883, 0.99996, 1 },
		{ 4.54747e-13, 8.11065e-10, 7.84415e-08, 2.37445e-06, 4.08044e-05, 0.000449365, 0.0033832, 0.0180593, 0.0698561, 0.198811, 0.422431, 0.685308, 0.885236, 0.976225, 0.997883, 0.99996, 1 }
	},
	{ //hybridbits=24
		{ 5.81564e-11, 1.16313e-10, 3.78017e-09, 9.51439e-08, 1.60628e-06, 1.91083e-05, 0.000167675, 0.00110309, 0.00554545, 0.0215918, 0.0659494, 0.160338, 0.315733, 0.514533, 0.712154, 0.863558, 0.951121 },
		{ 9.09495e-13, 1.73841e-10, 5.33113e-09, 1.87053e-07, 3.59138e-06, 4.79615e-05, 0.000453813, 0.00312783, 0.0159388, 0.060556, 0.172671, 0.372964, 0.621914, 0.831455, 0.947563, 0.98903, 0.998501 },
		{ 9.09495e-13, 2.31175e-10, 1.02873e-08, 4.24783e-07, 8.77059e-06, 0.000117139, 0.00107762, 0.00704785, 0.0333191, 0.114989, 0.292021, 0.552584, 0.801964, 0.947027, 0.993058, 0.999691, 0.999998 },
		{ 9.09495e-13, 5.22999e-10, 2.38255e-08, 8.36043e-07, 1.6816e-05, 0.000217438, 0.00189924, 0.0116356, 0.0509711, 0.161624, 0.374962, 0.648277, 0.869343, 0.973209, 0.997773, 0.999969, 1 },
		{ 9.09495e-13, 2.31514e-10, 3.85471e-08, 1.31836e-06, 2.57042e-05, 0.000313903, 0.00259147, 0.0149941, 0.0620968, 0.18652, 0.411575, 0.681962, 0.887195, 0.97793, 0.99824, 0.999976, 1 },
		{ 9.09495e-13, 8.10908e-10, 5.39254e-08, 1.89706e-06, 3.44525e-05, 0.000400072, 0.00314857, 0.0174234, 0.0692314, 0.200462, 0.429024, 0.695128, 0.89256, 0.978897, 0.998289, 0.999976, 1 },
		{ 9.09495e-13, 1.22116e-09, 7.83871e-08, 2.42843e-06, 4.20506e-05, 0.000466444, 0.00353379, 0.0189203, 0.0731301, 0.207076, 0.436029, 0.699383, 0.89385, 0.979043, 0.998291, 0.999976, 1 },
		{ 9.09495e-13, 1.67712e-09, 1.02651e-07, 2.9349e-06, 4.84731e-05, 0.000518019, 0.0038007, 0.0198441, 0.0752375, 0.210132, 0.438683, 0.700628, 0.894103, 0.979057, 0.998291, 0.999976, 1 }
	},
	{ //hybridbits=25
		{ 1.81899e-12, 5.80085e-11, 7.77314e-09, 1.71473e-07, 2.73528e-06, 3.11033e-05, 0.000261493, 0.00165152, 0.00796493, 0.0297171, 0.0868657, 0.201759, 0.378996, 0.588939, 0.778972, 0.908427, 0.972745 },
		{ 1.81899e-12, 5.81795e-11, 1.04723e-08, 3.06199e-07, 5.58767e-06, 7.01715e-05, 0.000629193, 0.00411913, 0.0199672, 0.0723029, 0.196976, 0.407974, 0.656208, 0.853705, 0.956922, 0.991543, 0.998921 },
		{ 1.81899e-12, 2.31459e-10, 2.00212e-08, 6.47681e-07, 1.21229e-05, 0.000153195, 0.00134277, 0.00840966, 0.038251, 0.127504, 0.313763, 0.57752, 0.819777, 0.954239, 0.994476, 0.999792, 0.999999 },
		{ 1.81899e-12, 4.64329e-10, 3.0994e-08, 1.09599e-06, 2.04742e-05, 0.000249745, 0.00209513, 0.0124384, 0.0532071, 0.165795, 0.379975, 0.651947, 0.870812, 0.973472, 0.997788, 0.999969, 1 },
		{ 1.81899e-12, 6.98079e-10, 5.36939e-08, 1.72199e-06, 3.12504e-05, 0.000364873, 0.00290446, 0.0163155, 0.0659303, 0.194057, 0.421323, 0.68985, 0.890863, 0.978779, 0.998313, 0.999977, 1 },
		{ 1.81899e-12, 6.94924e-10, 7.73682e-08, 2.33361e-06, 4.04545e-05, 0.000452275, 0.00345191, 0.018648, 0.0726612, 0.207015, 0.437376, 0.701863, 0.895729, 0.979651, 0.998357, 0.999977, 1 },
		{ 1.81899e-12, 1.16234e-09, 1.02227e-07, 2.94576e-06, 4.88387e-05, 0.000523552, 0.00386107, 0.0202234, 0.0767229, 0.213875, 0.444606, 0.706243, 0.897058, 0.9798, 0.998359, 0.999977, 1 },
		{ 1.81899e-12, 2.72289e-09, 1.26643e-07, 3.47545e-06, 5.51699e-05, 0.000573727, 0.00412114, 0.0211155, 0.0787553, 0.216818, 0.447161, 0.707437, 0.897303, 0.979813, 0.998359, 0.999977, 1 }
	},
	{ //hybridbits=26
		{ 3.63798e-12, 1.15929e-10, 1.26362e-08, 2.94749e-07, 4.56215e-06, 5.02484e-05, 0.000405291, 0.00245381, 0.0113336, 0.0404301, 0.112722, 0.249171, 0.444962, 0.658294, 0.833505, 0.939726, 0.985214 },
		{ 3.63798e-12, 3.48847e-10, 1.90121e-08, 4.97223e-07, 8.56366e-06, 0.000101984, 0.000867497, 0.00540446, 0.0249967, 0.0865658, 0.226056, 0.449941, 0.698116, 0.881862, 0.969255, 0.994975, 0.999519 },
		{ 3.63798e-12, 9.28203e-10, 3.13269e-08, 9.2275e-07, 1.65437e-05, 0.000197138, 0.00164234, 0.00983813, 0.04301, 0.138454, 0.330734, 0.594462, 0.829913, 0.957474, 0.99492, 0.999809, 0.999999 },
		{ 3.63798e-12, 7.54773e-10, 5.00473e-08, 1.4649e-06, 2.58339e-05, 0.000299919, 0.00241307, 0.0138332, 0.0574637, 0.174685, 0.39234, 0.662821, 0.87639, 0.974922, 0.99793, 0.999971, 1 },
		{ 3.63798e-12, 1.33606e-09, 7.30769e-08, 2.10448e-06, 3.59023e-05, 0.000401054, 0.00309253, 0.0169754, 0.0674885, 0.19648, 0.423682, 0.691175, 0.891239, 0.978818, 0.998314, 0.999977, 1 },
		{ 3.63798e-12, 1.39523e-09, 9.76082e-08, 2.85354e-06, 4.67359e-05, 0.000502065, 0.00372368, 0.0196531, 0.0752124, 0.211367, 0.442167, 0.705043, 0.896876, 0.979835, 0.998365, 0.999977, 1 },
		{ 3.63798e-12, 1.44896e-09, 1.30175e-07, 3.43711e-06, 5.4711e-05, 0.000569177, 0.00410195, 0.0211002, 0.078929, 0.217613, 0.448727, 0.709015, 0.898079, 0.979968, 0.998367, 0.999977, 1 },
		{ 3.63798e-12, 3.02318e-09, 1.56101e-07, 3.95873e-06, 6.11635e-05, 0.000621216, 0.00437201, 0.0220367, 0.0810619, 0.220711, 0.451417, 0.710279, 0.898341, 0.979983, 0.998367, 0.999977, 1 }
	},
	{ //hybridbits=27
		{ 7.27596e-12, 5.22337e-10, 2.3215e-08, 5.14734e-07, 7.63575e-06, 8.05662e-05, 0.000623944, 0.00362137, 0.0160089, 0.0545568, 0.144993, 0.304873, 0.51732, 0.72831, 0.882843, 0.963975, 0.992925 },
		{ 7.27596e-12, 4.07432e-10, 3.38168e-08, 8.06482e-07, 1.30436e-05, 0.000146816, 0.00118536, 0.00702694, 0.0309917, 0.102532, 0.25631, 0.489843, 0.733608, 0.902271, 0.976467, 0.996449, 0.999676 },
		{ 7.27596e-12, 9.88124e-10, 5.23706e-08, 1.33606e-06, 2.23606e-05, 0.000251765, 0.00199838, 0.0114715, 0.0483195, 0.150585, 0.349825, 0.614458, 0.843135, 0.962525, 0.995874, 0.999873, 1 },
		{ 7.27596e-12, 1.10048e-09, 7.28633e-08, 1.95607e-06, 3.23991e-05, 0.000356365, 0.00274303, 0.0151711, 0.0612066, 0.181771, 0.40108, 0.669414, 0.879123, 0.975431, 0.997957, 0.999971, 1 },
		{ 7.27596e-12, 2.08616e-09, 9.67169e-08, 2.63674e-06, 4.25598e-05, 0.00045381, 0.00338371, 0.0180873, 0.070406, 0.201663, 0.429695, 0.695449, 0.892912, 0.979113, 0.998329, 0.999977, 1 },
		{ 5.81471e-11, 3.19809e-09, 1.22632e-07, 3.28328e-06, 5.12097e-05, 0.000533005, 0.00386722, 0.020099, 0.0761342, 0.212583, 0.443138, 0.705464, 0.89696, 0.979838, 0.998365, 0.999977, 1 },
		{ 5.79048e-11, 2.31619e-09, 1.47599e-07, 3.90186e-06, 6.0078e-05, 0.00060966, 0.00430526, 0.0217915, 0.0805037, 0.219975, 0.45094, 0.710209, 0.898397, 0.98, 0.998367, 0.999977, 1 },
		{ 7.27596e-12, 2.96254e-09, 1.72292e-07, 4.38532e-06, 6.60562e-05, 0.000657061, 0.00455084, 0.0226397, 0.082433, 0.222764, 0.453354, 0.711341, 0.898633, 0.980015, 0.998367, 0.999977, 1 }
	},
	{ //hybridbits=28
		{ 1.45519e-11, 1.50996e-09, 3.82717e-08, 8.82049e-07, 1.27723e-05, 0.0001289, 0.000955674, 0.00531214, 0.0224615, 0.0730585, 0.184819, 0.368993, 0.594043, 0.795754, 0.925328, 0.982153, 0.997653 },
		{ 5.79112e-11, 1.79525e-09, 5.33941e-08, 1.29628e-06, 1.97091e-05, 0.000209903, 0.00160809, 0.00908306, 0.0382559, 0.121098, 0.290179, 0.533066, 0.771166, 0.923806, 0.984352, 0.998228, 0.99991 },
		{ 1.45519e-11, 1.68634e-09, 8.18749e-08, 1.93075e-06, 3.00538e-05, 0.000320212, 0.00242383, 0.0133467, 0.05418, 0.163409, 0.368992, 0.633148, 0.854154, 0.965982, 0.996324, 0.999886, 1 },
		{ 1.45519e-11, 2.77371e-09, 1.09504e-07, 2.5744e-06, 4.02735e-05, 0.000421934, 0.00311814, 0.016672, 0.0654042, 0.189882, 0.411624, 0.678242, 0.883541, 0.976591, 0.998078, 0.999973, 1 },
		{ 1.45519e-11, 2.72622e-09, 1.26334e-07, 3.19966e-06, 4.92021e-05, 0.000506738, 0.00366315, 0.019103, 0.0729151, 0.205765, 0.433932, 0.69801, 0.893698, 0.979206, 0.99833, 0.999977, 1 },
		{ 1.45519e-11, 4.06862e-09, 1.58618e-07, 3.76714e-06, 5.72236e-05, 0.00057811, 0.00409554, 0.0208945, 0.0780154, 0.215535, 0.446067, 0.707157, 0.897452, 0.979891, 0.998366, 0.999977, 1 },
		{ 5.78758e-11, 4.39856e-09, 1.78142e-07, 4.30938e-06, 6.40565e-05, 0.000635607, 0.00441478, 0.0221017, 0.0810736, 0.220625, 0.451374, 0.710358, 0.898416, 0.980001, 0.998367, 0.999977, 1 },
		{ 5.81287e-11, 4.01088e-09, 1.98684e-07, 4.85171e-06, 7.09795e-05, 0.000691507, 0.00470876, 0.0231219, 0.0834126, 0.224038, 0.45435, 0.711753, 0.898709, 0.980017, 0.998367, 0.999977, 1 }
	},
	{ //hybridbits=29
		{ 2.91038e-11, 2.20976e-09, 7.12356e-08, 1.53945e-06, 2.11377e-05, 0.000204462, 0.00145192, 0.00771934, 0.0311642, 0.0965164, 0.231761, 0.437923, 0.666936, 0.849614, 0.951855, 0.990214, 0.998981 },
		{ 2.91038e-11, 2.61872e-09, 9.77654e-08, 2.03335e-06, 2.94163e-05, 0.00029592, 0.00215373, 0.0115789, 0.0465296, 0.140881, 0.323782, 0.572573, 0.802031, 0.938859, 0.988502, 0.998775, 0.999934 },
		{ 2.91038e-11, 2.66357e-09, 1.17081e-07, 2.69826e-06, 3.95576e-05, 0.000399832, 0.00289601, 0.0153389, 0.0601423, 0.175959, 0.387138, 0.650554, 0.864652, 0.969647, 0.996966, 0.999926, 1 },
		{ 1.15623e-10, 3.58432e-09, 1.44876e-07, 3.27832e-06, 4.83521e-05, 0.000486788, 0.00347876, 0.0180795, 0.0692377, 0.197044, 0.420488, 0.685066, 0.886484, 0.977173, 0.998111, 0.999973, 1 },
		{ 2.91038e-11, 3.53621e-09, 1.62897e-07, 3.84061e-06, 5.62131e-05, 0.000559843, 0.00393964, 0.0201027, 0.0754171, 0.210027, 0.438694, 0.701323, 0.894998, 0.979446, 0.998343, 0.999977, 1 },
		{ 2.91038e-11, 4.59046e-09, 1.87163e-07, 4.33281e-06, 6.31693e-05, 0.000620548, 0.00429959, 0.0215609, 0.0794685, 0.217584, 0.447811, 0.707974, 0.897622, 0.979901, 0.998365, 0.999977, 1 },
		{ 2.91038e-11, 4.10862e-09, 2.02769e-07, 4.78452e-06, 6.93718e-05, 0.000671348, 0.00458183, 0.0226242, 0.082175, 0.222134, 0.452621, 0.710921, 0.898532, 0.980009, 0.998368, 0.999977, 1 },
		{ 1.16157e-10, 5.98209e-09, 2.27958e-07, 5.23276e-06, 7.44192e-05, 0.000710392, 0.00478033, 0.0232995, 0.0836905, 0.224299, 0.45448, 0.711782, 0.898708, 0.980017, 0.998368, 0.999977, 1 }
	},
	{ //hybridbits=30
		{ 5.82077e-11, 3.82978e-09, 1.34449e-07, 2.66332e-06, 3.48874e-05, 0.000323654, 0.00220493, 0.0112335, 0.0433609, 0.128039, 0.292097, 0.522551, 0.752303, 0.909557, 0.979427, 0.997633, 0.999914 },
		{ 5.82077e-11, 4.68368e-09, 1.5566e-07, 3.24856e-06, 4.39644e-05, 0.000419192, 0.00290604, 0.0149114, 0.0572743, 0.165868, 0.365033, 0.619717, 0.838056, 0.956485, 0.99373, 0.999634, 0.999996 },
		{ 5.82077e-11, 4.58694e-09, 1.75117e-07, 3.75816e-06, 5.19217e-05, 0.000500231, 0.00347088, 0.0176932, 0.067049, 0.190228, 0.407368, 0.669363, 0.875299, 0.972869, 0.997367, 0.999936, 1 },
		{ 5.81557e-11, 4.82693e-09, 2.04592e-07, 4.26974e-06, 5.90954e-05, 0.000568925, 0.00392103, 0.0197671, 0.0737783, 0.205518, 0.431124, 0.693616, 0.890567, 0.978199, 0.998216, 0.999975, 1 },
		{ 5.79818e-11, 4.75451e-09, 2.12851e-07, 4.70018e-06, 6.5563e-05, 0.000626235, 0.00427538, 0.0212867, 0.0783183, 0.214823, 0.44383, 0.704605, 0.896099, 0.979595, 0.998348, 0.999977, 1 },
		{ 5.82077e-11, 6.14185e-09, 2.34086e-07, 5.13737e-06, 7.11324e-05, 0.000674385, 0.00455145, 0.0223825, 0.0813093, 0.220342, 0.450455, 0.709476, 0.898066, 0.979953, 0.998367, 0.999977, 1 },
		{ 5.81722e-11, 6.45711e-09, 2.52176e-07, 5.45347e-06, 7.58513e-05, 0.000712774, 0.00476207, 0.0231527, 0.0832008, 0.22338, 0.453518, 0.711255, 0.898585, 0.98001, 0.998367, 0.999977, 1 },
		{ 5.80644e-11, 7.14192e-09, 2.65993e-07, 5.84737e-06, 8.01932e-05, 0.000745844, 0.00492677, 0.0237104, 0.0844467, 0.22517, 0.455063, 0.711982, 0.898734, 0.980018, 0.998367, 0.999977, 1 }
	},
	{ //hybridbits=31
		{ 1.74583e-10, 7.97262e-09, 2.44882e-07, 4.52286e-06, 5.65677e-05, 0.000499675, 0.00324023, 0.015705, 0.0575597, 0.160929, 0.346723, 0.585794, 0.801144, 0.933102, 0.985838, 0.998481, 0.999954 },
		{ 1.15736e-10, 8.73809e-09, 2.57513e-07, 4.90265e-06, 6.24202e-05, 0.000563233, 0.00370826, 0.0181589, 0.0668392, 0.186145, 0.39534, 0.65055, 0.858259, 0.964248, 0.995195, 0.999722, 0.999996 },
		{ 3.47098e-10, 8.21466e-09, 2.72704e-07, 5.27289e-06, 6.77486e-05, 0.000616876, 0.00408343, 0.020012, 0.0733511, 0.202388, 0.423572, 0.68368, 0.883189, 0.975341, 0.997747, 0.999956, 1 },
		{ 2.32047e-10, 9.804e-09, 2.90001e-07, 5.58079e-06, 7.26069e-05, 0.00066301, 0.00438382, 0.0213943, 0.077842, 0.212579, 0.439382, 0.699772, 0.893216, 0.97874, 0.99825, 0.999975, 1 },
		{ 2.32071e-10, 8.23852e-09, 2.98153e-07, 5.92054e-06, 7.6869e-05, 0.000701315, 0.00462085, 0.0224088, 0.0808684, 0.218788, 0.447906, 0.707227, 0.897059, 0.979765, 0.998358, 0.999977, 1 },
		{ 5.77809e-11, 8.55158e-09, 3.07395e-07, 6.17441e-06, 8.05832e-05, 0.000732778, 0.00480356, 0.0231375, 0.0828562, 0.222433, 0.45222, 0.710309, 0.898249, 0.979965, 0.998367, 0.999977, 1 },
		{ 1.16415e-10, 9.06363e-09, 3.17808e-07, 6.40397e-06, 8.37667e-05, 0.000759162, 0.0049448, 0.0236534, 0.0841375, 0.224535, 0.4544, 0.711636, 0.89866, 0.980014, 0.998367, 0.999977, 1 },
		{ 1.15758e-10, 9.14487e-09, 3.37318e-07, 6.64513e-06, 8.66771e-05, 0.000780467, 0.005052, 0.0240086, 0.0849089, 0.225608, 0.455286, 0.712031, 0.898738, 0.980019, 0.998367, 0.999977, 1 }
	},
	{ //hybridbits=32
		{ 3.489e-10, 1.48864e-08, 4.53512e-07, 8.30015e-06, 9.95806e-05, 0.000851372, 0.00531098, 0.024651, 0.085961, 0.226695, 0.455956, 0.712246, 0.898763, 0.980019, 0.998367, 0.999977, 1 },
		{ 2.89786e-10, 1.58803e-08, 4.52935e-07, 8.27588e-06, 9.97052e-05, 0.00085074, 0.00531087, 0.0246505, 0.0859629, 0.226698, 0.45595, 0.712244, 0.898765, 0.980019, 0.998367, 0.999977, 1 },
		{ 3.4646e-10, 1.65146e-08, 4.6339e-07, 8.24546e-06, 9.96506e-05, 0.000851186, 0.00531139, 0.0246513, 0.0859592, 0.226695, 0.455956, 0.712244, 0.898762, 0.980018, 0.998367, 0.999977, 1 },
		{ 1.73915e-10, 1.57683e-08, 4.56643e-07, 8.2732e-06, 9.97015e-05, 0.000850903, 0.00531102, 0.0246504, 0.0859635, 0.226696, 0.455951, 0.712238, 0.89876, 0.980018, 0.998367, 0.999977, 1 },
		{ 4.04258e-10, 1.65746e-08, 4.61317e-07, 8.28834e-06, 9.96631e-05, 0.00085106, 0.00531132, 0.0246506, 0.0859579, 0.226697, 0.455956, 0.712236, 0.898763, 0.980018, 0.998367, 0.999977, 1 },
		{ 3.48799e-10, 1.55797e-08, 4.61752e-07, 8.29304e-06, 9.95871e-05, 0.000850873, 0.00531056, 0.0246517, 0.0859626, 0.226697, 0.455957, 0.712243, 0.89876, 0.980019, 0.998368, 0.999977, 1 },
		{ 2.31903e-10, 1.59433e-08, 4.64617e-07, 8.28594e-06, 9.96503e-05, 0.00085112, 0.00531135, 0.0246511, 0.0859633, 0.226694, 0.45595, 0.712237, 0.898761, 0.980018, 0.998367, 0.999977, 1 },
		{ 2.88863e-10, 1.53675e-08, 4.59928e-07, 8.27021e-06, 9.97239e-05, 0.000850977, 0.00531107, 0.0246502, 0.0859589, 0.226694, 0.455949, 0.712236, 0.89876, 0.980017, 0.998367, 0.999977, 1 }
	}
};

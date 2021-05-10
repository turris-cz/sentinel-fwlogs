divert(-1)
define(`foreach',`ifelse(eval($#>2),1,`pushdef(`$1',`$3')$2`'popdef(`$1')`'ifelse(eval($#>3),1,`$0(`$1',`$2',shift(shift(shift($@))))')')')
divert(0)dnl

static const char *protocol2str(unsigned protocol) {
	switch (protocol) {
foreach(`PROTO_NAME',``#'ifdef IPPROTO_`'PROTO_NAME
		case IPPROTO_`'PROTO_NAME:
			return "PROTO_NAME";
#endif'
,esyscmd(`sed -n "s/^\([^ \t#]\+\).*$/\1/p" protocols | paste -sd "," | tr -d "\n"'))dnl
		default:
			return "Unknown";
	};
}

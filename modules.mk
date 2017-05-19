mod_mptcp.la: mod_mptcp.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_mptcp.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_mptcp.la

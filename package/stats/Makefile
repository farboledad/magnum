include $(TOPDIR)/rules.mk

PKG_NAME:=stats
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/stats
  SECTION:=network
  CATEGORY:=Network
  TITLE:=Stats tool
  DEPENDS:=+sshtunn +darkstat +vnstat +vnstati +luvit-startup
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -l src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/stats $(1)/bin/
endef

define Package/stats/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/stats $(1)/bin/
	$(INSTALL_DIR) $(1)/www
	$(INSTALL_DIR) $(1)/www/stats
	$(CP) ./www/* $(1)/www/stats
	$(INSTALL_DIR) $(1)/etc
	$(CP) ./files/vnstatigenerate $(1)/etc
	$(CP) ./files/darkstatgenerate $(1)/etc
	$(CP) ./files/backupdata $(1)/etc
	$(CP) ./files/sshtunn_install $(1)/etc
	$(CP) ./files/stats_install $(1)/etc
	$(CP) ./files/luvitred2_install $(1)/etc
	$(CP) ./files/hosts_csvgen $(1)/etc
	$(CP) ./files/hosts_bck_csvgen $(1)/etc
	$(CP) ./files/vnstat_dumped.csv $(1)/etc
	$(CP) ./files/loader $(1)
	$(CP) ./files/isv $(1)
endef

$(eval $(call BuildPackage,stats))

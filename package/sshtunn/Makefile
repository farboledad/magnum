include $(TOPDIR)/rules.mk

PKG_NAME:=sshtunn
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/sshtunn
  SECTION:=network
  CATEGORY:=Network
  TITLE:=SSH Tunnel
  DEPENDS:=+jansson +libcg +autossh +root_enabler
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
	$(CP) $(PKG_BUILD_DIR)/sshtunn $(1)/bin/
endef

define Package/sshtunn/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/sshtunn $(1)/bin/
	$(CP) ./files/wrapper $(1)/bin
	$(CP) ./files/internet_status_check $(1)/bin/
	$(INSTALL_DIR) $(1)/www
	$(CP) ./www/* $(1)/www
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) ./files/sshtunn.init $(1)/etc/init.d/sshtunn
	$(CP) ./files/rc.common $(1)/etc/
endef

$(eval $(call BuildPackage,sshtunn))

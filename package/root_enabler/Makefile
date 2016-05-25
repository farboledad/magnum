include $(TOPDIR)/rules.mk

PKG_NAME:=root_enabler
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/root_enabler
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=Root enabler
  DEPENDS:=+libcg
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -l src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS) -lcg -lpthread"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/root_enabler $(1)/bin/
endef

define Package/root_enabler/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/root_enabler $(1)/bin/
endef

$(eval $(call BuildPackage,root_enabler))

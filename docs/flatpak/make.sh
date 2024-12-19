#!/bin/bash
flatpak-builder --force-clean --user --install-deps-from=flathub --repo=repo dist com.karurochari.vs.yml 
flatpak build-bundle repo vs.flatpak com.karurochari.vs
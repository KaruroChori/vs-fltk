#!/bin/bash
flatpak run org.flatpak.Builder --force-clean --install-deps-from=flathub --repo=repo dist com.karurochari.vs.yml 
flatpak build-bundle repo vs.flatpak com.karurochari.vs
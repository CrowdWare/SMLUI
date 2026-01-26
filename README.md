# SMLUI

SMLUI mappt SML (SimpleMarkupLanguage) auf ImGui. Ziel: UI-Layouts in Text beschreiben und im Editor rendern.

## Was es kann
- Liest `Window`, `DockLayout`, `ToolBar`, `StatusBar`, `PropertyPanel`, `Viewport3D`, `Overlay`
- Erzeugt Toolbar-Buttons aus SML-Icons (z.B. `play`, `select`, `move`, `paint`)
- Rendert ein Docking-Layout in ImGui

## Beispiel (SML)
```sml
Window {
    title: "RaidBuilder"
    size: 1280,720
    DockLayout {
        Left { label: "Tools" width: 56 }
        Right { label: "Properties" width: 360 }
        Center { label: "Viewport" }
    }
}
```

## Build
Voraussetzungen:
- Vulkan SDK
- GLFW3 (pkg-config)

```sh
cmake -S . -B build
cmake --build build
```

## Hinweise
- ImGui ist lokal eingebunden (`imgui/`).
- Abhaengig von `SMLParser`.

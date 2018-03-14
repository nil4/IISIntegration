$WindowsIdentity = [system.security.principal.windowsidentity]::GetCurrent()
$Principal = New-Object System.Security.Principal.WindowsPrincipal($WindowsIdentity)
$AdminRole = [System.Security.Principal.WindowsBuiltInRole]::Administrator
if ($Principal.IsInRole($AdminRole))
{
    Write-Host -ForegroundColor Green "Elevated PowerShell session detected. Continuing."
}
else
{
    Write-Host -ForegroundColor Red "This application/script must be run in an elevated PowerShell window. Please launch an elevated session and try again."
    Break
}

Import-Module IISAdministration

function GetModuleCollection
{
    $ConfigSection = Get-IISConfigSection -SectionPath "system.webServer/globalModules"
    return Get-IISConfigCollection -ConfigElement $ConfigSection
}

function GetANCMModule
{
    return  Get-IISConfigCollectionElement -ConfigCollection (GetModuleCollection) -ConfigAttribute @{"name" = "AspNetCoreModule"}
}
function GetOldANCMModule
{
    return  Get-IISConfigCollectionElement -ConfigCollection (GetModuleCollection) -ConfigAttribute @{"name" = "OldAspNetCoreModule"}
}
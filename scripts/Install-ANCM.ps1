param($ModuleImage)

. "$PSScriptRoot\common.ps1"

Start-IISCommitDelay

$ModulesCollection = GetModuleCollection;

$OldModule = GetOldANCMModule;
if ($OldModule)
{
    Write-Host -ForegroundColor Red "OldAspNetCoreModule module exists, delete it before installing."
    Break
}

$Module = GetANCMModule;
if ($Module)
{
    Write-Host "Found existing element renaming to OldAspNetCoreModule";
    Set-IISConfigAttributeValue -ConfigElement $Module -AttributeName "name" -AttributeValue "OldAspNetCoreModule"
}

Stop-IISCommitDelay -Commit $True


Start-IISCommitDelay

Write-Host "Adding new module AspNetCoreModule with image path $ModuleImage";
$ModulesCollection = GetModuleCollection;
New-IISConfigCollectionElement -ConfigCollection $ModulesCollection -ConfigAttribute @{"name" = "AspNetCoreModule"; "image" = $ModuleImage}

Stop-IISCommitDelay -Commit $True
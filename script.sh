echo "[INICIANDO EXECUÇÃO]"
echo ""
echo "[REMOVENDO DIRETÓRIO OUTPUT]"
rm -rf output
echo "[CRIANDO DIRETÓRIO OUTPUT]"
mkdir output
echo ""
echo "[FUNCIONALIDADES 1 E 2] [GERANDO ARQUIVOS DE RESPOSTA]"
./programaTrab1 1 'turmaB-dadosPBLE.csv' > output/resultado-funcionalidades1e2.txt 2>&1
./programaTrab1 2 >> output/resultado-funcionalidades1e2.txt 2>&1
rm -rf output.dat
echo "[FUNCIONALIDADES 1 E 2] [GERADO]"
echo ""
echo "[FUNCIONALIDADE 3] [GERANDO ARQUIVO DE RESPOSTA]"
./programaTrab1 1 'turmaB-dadosPBLE.csv' > output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'codINEP' 31031917 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'codINEP' 31034614 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'codINEP' 41357280 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'codINEP' 1 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'dataAtiv' 01/09/2010 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'dataAtiv' 20/12/2012 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'municipio' 'PATOS DE MINAS' >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'municipio' 'ICARAIMA' >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'municipio' 'CIDADE INEX' >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'prestadora' 'OI' >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 5 0 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 5 90 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 5 93 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 5 1876 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'municipio' 'PATOS DE MINAS' >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'codINEP' 31034614 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'codINEP' 31031917 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'codINEP' 41357280 >> output/resultado-funcionalidade3.txt 2>&1
./programaTrab1 3 'municipio' 'ICARAIMA' >> output/resultado-funcionalidade3.txt 2>&1
rm -rf output.dat
echo "[FUNCIONALIDADE 3] [GERADO]"
echo ""
echo "[FUNCIONALIDADE 4] [GERANDO ARQUIVO DE RESPOSTA]"
./programaTrab1 1 'turmaB-dadosPBLE.csv' > output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 0 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 1363 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 52 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 2363 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 163 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 133 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 136 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 113 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 2 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 5 0 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 5 1363 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 5 163 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 5 136 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 5 2 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 0 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 1363 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 52 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 2363 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 163 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 133 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 136 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 113 >> output/resultado-funcionalidade4.txt 2>&1
./programaTrab1 4 2 >> output/resultado-funcionalidade4.txt 2>&1
rm -rf output.dat
echo "[FUNCIONALIDADE 4] [GERADO]"
echo ""
echo "[FUNCIONALIDADE 5] [GERANDO ARQUIVO DE RESPOSTA]"
./programaTrab1 1 'turmaB-dadosPBLE.csv' > output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 2 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 0 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 1363 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 163 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 136 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 2 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 1363 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 1263 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 1336 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 232 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 399 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 402 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 106 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 22 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 13 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 16 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 1396 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 5 29 >> output/resultado-funcionalidade5.txt 2>&1
./programaTrab1 2 >> output/resultado-funcionalidade5.txt 2>&1
rm -rf output.dat
echo "[FUNCIONALIDADE 5] [GERADO]"
echo ""
echo "[FUNCIONALIDADES 6 E 9] [GERANDO ARQUIVO DE RESPOSTA]"
./programaTrab1 1 'turmaB-dadosPBLE.csv' > output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 0 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 1363 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 163 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 136 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 2 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 1263 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 1336 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 232 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 399 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 402 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 106 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 22 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 13 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 16 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 1396 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 5 29 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 2 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 9 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 6 111 10/03/2016 SP 'teste' 'teste' 'test' >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 6 112 10/03/2016 MG 'teste2' 'teste' 'test' >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 6 113 10/03/2016 RR 'teste3' '' 'test' >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 6 114 10/03/2016 AM 'teste' 'a' 'test' >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 6 115 10/03/2016 SP 'teste' 'teste' 'test' >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 6 116 10/03/2016 SP 'teste' 'd' 'test' >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 2 >> output/resultado-funcionalidades6e9.txt 2>&1
./programaTrab1 9 >> output/resultado-funcionalidades6e9.txt 2>&1
rm -rf output.dat
echo "[FUNCIONALIDADES 6 E 9] [GERADO]"
echo ""
echo "[FUNCIONALIDADE 7] [GERANDO ARQUIVO DE RESPOSTA]"
./programaTrab1 1 'turmaB-dadosPBLE.csv' > output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 0 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 1363 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 163 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 136 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 2 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 1263 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 1336 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 232 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 399 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 402 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 106 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 22 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 13 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 16 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 1396 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 5 29 >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 7 1 111 10/03/2016 SP 'teste' 'teste' 'test' >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 7 22 112 10/03/2016 MG 'teste2' 'teste' 'test' >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 7 20 113 10/03/2016 RR 'teste3' '' 'test' >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 7 43 114 10/03/2016 AM 'teste' 'a' 'test' >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 7 1369 115 10/03/2016 SP 'teste' 'teste' 'test' >> output/resultado-funcionalidade7.txt 2>&1
./programaTrab1 2 >> output/resultado-funcionalidade7.txt 2>&1
rm -rf output.dat
echo "[FUNCIONALIDADE 7] [GERADO]"
echo ""
echo "[FUNCIONALIDADE 8] [GERANDO ARQUIVO DE RESPOSTA]"
./programaTrab1 1 'turmaB-dadosPBLE.csv' > output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 0 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 1363 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 163 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 136 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 2 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 1263 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 1336 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 232 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 399 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 402 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 106 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 22 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 13 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 16 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 1396 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 5 29 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 8 >> output/resultado-funcionalidade8.txt 2>&1
./programaTrab1 2 >> output/resultado-funcionalidade8.txt 2>&1
rm -rf output.dat
echo "[FUNCIONALIDADE 8] [GERADO]"
echo "---"
echo "[EXECUÇÃO FINALIZADA]"
